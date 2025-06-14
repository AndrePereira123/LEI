const Item = require('../models/Item');
const User = require('../models/User');
const jwt = require('jsonwebtoken');
const ItemModel = require('../models/Item');

// GET /api/users/:id - Get user profile
exports.getUserProfile = async (req, res) => {
  try {
    const userId = req.params.id;
    
    if (req.user.id !== userId && req.user.tipo !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para aceder a este perfil'
      });
    }
    
    const user = await User.findById(userId).select('-salt -hash');
    
    if (!user) {
      return res.status(404).json({
        success: false,
        message: 'Utilizador não encontrado'
      });
    }
    
    res.status(200).json({
      success: true,
      user: {
        id: user._id,
        username: user.username,
        nome: user.nome,
        email: user.email,
        tipo: user.tipo,
        dataCriacao: user.dataCriacao
      }
    });
    
  } catch (error) {
    console.error('Erro ao obter perfil:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};

// PUT /api/users/:id - Update user profile
exports.updateUserProfile = async (req, res) => {
  try {
    const userId = req.params.id;
    const { nome, email, username, tipo } = req.body;
    
    console.log('Update user profile request:', { userId, nome, email, username, tipo });
    

    if (username === "Deleted User")
      return res.status(400).json({
        success: false,
        message: 'Username "Deleted User" não é permitido'
      });

      
    if (!userId || userId === 'undefined') {
      return res.status(400).json({
        success: false,
        message: 'ID do utilizador inválido'
      });
    }
    
    if (req.user.id !== userId && req.user.tipo !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para atualizar este perfil'
      });
    }
    
    if (!nome || !email || !username) {
      return res.status(400).json({
        success: false,
        message: 'Nome, email e username são obrigatórios'
      });
    }
    
    const existingUserWithEmail = await User.findOne({ 
      email: email, 
      _id: { $ne: userId } 
    });
    
    if (existingUserWithEmail) {
      return res.status(409).json({
        success: false,
        message: 'Este email já está a ser utilizado por outro utilizador'
      });
    }
    
    const existingUserWithUsername = await User.findOne({ 
      username: username, 
      _id: { $ne: userId } 
    });
    
    if (existingUserWithUsername) {
      return res.status(409).json({
        success: false,
        message: 'Este username já está a ser utilizado por outro utilizador'
      });
    }

    const updatedUser = await User.findByIdAndUpdate(
      userId,
      { nome, email, username, tipo },
      { new: true, runValidators: true }
    ).select('-salt -hash');
    
    if (!updatedUser) {
      return res.status(404).json({
        success: false,
        message: 'Utilizador não encontrado'
      });
    }

      
    await ItemModel.updateMany(
      { 'comentarios.autorId': userId },
      { 
        $set: { 
          'comentarios.$[elem].autor': username 
        }
      },
      {
        arrayFilters: [{ 'elem.autorId': userId }]
      }
    );
    
    res.status(200).json({
      success: true,
      message: 'Perfil atualizado com sucesso',
      user: {
        id: updatedUser._id,
        username: updatedUser.username,
        nome: updatedUser.nome,
        email: updatedUser.email,
        tipo: updatedUser.tipo,
        dataCriacao: updatedUser.dataCriacao
      }
    });
    
  } catch (error) {
    console.error('Erro ao atualizar perfil:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};

// POST /api/users/:id/change-password - Change user password
exports.changePassword = async (req, res) => {
  try {
    const userId = req.params.id;
    const { currentPassword, newPassword, confirmPassword } = req.body;
    
    if (req.user.id !== userId) {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para alterar esta password'
      });
    }
    
    if (!currentPassword || !newPassword || !confirmPassword) {
      return res.status(400).json({
        success: false,
        message: 'Todos os campos são obrigatórios'
      });
    }
    
    if (newPassword !== confirmPassword) {
      return res.status(400).json({
        success: false,
        message: 'As passwords não coincidem'
      });
    }
    
    if (newPassword.length < 6) {
      return res.status(400).json({
        success: false,
        message: 'A nova password deve ter pelo menos 6 caracteres'
      });
    }
    
    const user = await User.findById(userId);
    
    if (!user) {
      return res.status(404).json({
        success: false,
        message: 'Utilizador não encontrado'
      });
    }
    
    user.authenticate(currentPassword, (err, thisModel, passwordErr) => {
      if (passwordErr) {
        return res.status(401).json({
          success: false,
          message: 'Password atual incorreta'
        });
      }
      
      user.changePassword(currentPassword, newPassword, (err) => {
        if (err) {
          console.error('Erro ao alterar password:', err);
          return res.status(500).json({
            success: false,
            message: 'Erro ao alterar password'
          });
        }
        
        res.status(200).json({
          success: true,
          message: 'Password alterada com sucesso'
        });
      });
    });
      } catch (error) {
    console.error('Erro ao alterar password:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};

// GET /api/users/public/:username - Get public user profile by username
exports.getPublicUserProfile = async (req, res) => {
  try {
    const username = req.params.username;
    
    const user = await User.findOne({ username: username }).select('-salt -hash -email');
    
    if (!user) {
      return res.status(404).json({
        success: false,
        message: 'Utilizador não encontrado'
      });
    }
    
    res.status(200).json({
      success: true,
      user: {
        id: user._id,
        username: user.username,
        nome: user.nome,
        tipo: user.tipo,
        dataCriacao: user.dataCriacao
      }
    });
    
  } catch (error) {
    console.error('Erro ao obter perfil público:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};

// GET /api/users - Get all users (admin only)
exports.getAllUsers = async (req, res) => {
  try {
    if (req.user.tipo !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Apenas administradores podem aceder a esta funcionalidade'
      });
    }
      
    const users = await User.find({}).select('-salt -hash').sort({ dataCriacao: -1 });
    
    const formattedUsers = users.map(user => ({
      id: user._id,
      _id: user._id, 
      username: user.username,
      nome: user.nome,
      email: user.email,
      tipo: user.tipo,
      dataCriacao: user.dataCriacao
    }));
    
    res.status(200).json(formattedUsers);
    
  } catch (error) {
    console.error('Erro ao obter todos os utilizadores:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};

// DELETE /api/users/:id - Delete user and all their posts (admin only)
exports.deleteUser = async (req, res) => {
  try {
    const userId = req.params.id;
    
    if (req.user.tipo !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Apenas administradores podem eliminar utilizadores'
      });
    }
    
    if (req.user.id === userId) {
      return res.status(400).json({
        success: false,
        message: 'Não pode eliminar a sua própria conta'
      });
    }
    
    const user = await User.findById(userId);
    
    if (!user) {
      return res.status(404).json({
        success: false,
        message: 'Utilizador não encontrado'
      });
    }


    await ItemModel.updateMany(
      { 'comentarios.autorId': userId },
      { 
        $set: { 
          'comentarios.$[elem].autor': "Deleted User" 
        }
      },
      {
        arrayFilters: [{ 'elem.autorId': userId }]
      }
    );
    
    const deletedItems = await ItemModel.deleteMany({ submetidoPor: userId });
    console.log(`Deleted ${deletedItems.deletedCount} posts from user ${user.username}`);
    
    await User.findByIdAndDelete(userId);
    console.log(`User ${user.username} deleted successfully`);
    
    res.status(200).json({
      success: true,
      message: `Utilizador ${user.nome} e todos os seus posts foram eliminados com sucesso`,
      deletedPostsCount: deletedItems.deletedCount
    });
    
  } catch (error) {
    console.error('Erro ao eliminar utilizador:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
};
