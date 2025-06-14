
var express = require('express');
var router = express.Router();
const axios = require('axios'); 
const { requireAuth, requireAdmin } = require('../middleware/auth');
const multer = require('multer'); 
const FormData = require('form-data'); 
const fs = require('fs'); 


/* GET home page. */
router.get('/', function(req, res, next) {
  res.redirect('/homepage');
});

router.get('/sip/upload', requireAuth, function(req, res, next) {
  res.render('Sip_upload', {
    title: 'Upload SIP - Meu Eu Digital',
    backendUrl: process.env.BACKEND_URL, // Passar URL do backend para o template
    success: req.query.success,  // Passa o parâmetro success
    error: req.query.error
  });
});

router.get('/homepage', async function(req, res, next) {
  try {
    // Chamar o backend para obter dados
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;

    console.log(`ligando ao backend: ${backendUrl}/api/items/homepage-data`);
    
    const headers = {};
    if (req.cookies.token) {
      headers['Authorization'] = `Bearer ${req.cookies.token}`;
    }

    const response = await axios.get(`${backendUrl}/api/items/homepage-data`, {
      headers: headers
    });
    
    const homepageData = response.data;
    console.log('Dados da homepage recebidos:', homepageData);

    // Renderizar a página com os dados do backend
    res.render('homepage', { 
      title: 'Homepage',
      data: homepageData
    });
    
  } catch (error) {
    console.error('Erro ao chamar backend:', error.message);
    
    res.render('error', {
    error : {
      status: 500,
      message: 'Erro ao obter dados do backend'
    }});
  }
});

router.get('/search', async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const { q, categoria } = req.query;
    
    console.log(`Searching items with query: ${q}, category: ${categoria}`);
    
    const headers = {};
    if (req.cookies.token) {
      headers['Authorization'] = `Bearer ${req.cookies.token}`;
    }
    
    // Build query parameters
    const params = {};
    if (q) params.q = q;
    if (categoria) params.categoria = categoria;
    
    // Se há parâmetros de pesquisa, buscar no backend
    let searchResults = null;
    if (q || categoria) {
      const response = await axios.get(`${backendUrl}/api/items/search`, {
        headers: headers,
        params: params
      });
      searchResults = response.data;
      console.log('Search results received:', searchResults);
    }
    
    // Render search page 
    res.render('search', { 
      title: 'Pesquisar - Meu Eu Digital',
      searchResults: searchResults,
      searchQuery: q || '',
      selectedCategory: categoria || ''
    });
    
  } catch (error) {
    console.error('Erro ao renderizar página de pesquisa:', error.message);
    
    res.render('search', {
      title: 'Pesquisar - Meu Eu Digital',
      error: 'Erro ao realizar pesquisa: ' + (error.response?.data?.message || error.message),
      searchQuery: req.query.q || '',
      selectedCategory: req.query.categoria || ''
    });
  }
});

// Profile routes
router.get('/profile', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.user.id;    

      const response = await axios.get(`${backendUrl}/api/users/${userId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    const userData = response.data;
    const user = userData.success ? userData.user : userData;
    console.log('Profile data received:', user);

    res.render('profile', { 
      title: 'Perfil - Meu Eu Digital',
      user: user,
      success: req.query.success,
      error: req.query.error
    });
    
  } catch (error) {
    console.error('Error fetching profile:', error.message);
      res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar perfil'
      }
    });
  }
});

router.post('/profile/update', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.user.id;

    console.log(`Updating profile for user ${userId}`);
      const response = await axios.put(`${backendUrl}/api/users/${userId}`, req.body, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`,
        'Content-Type': 'application/json'
      }
    });
    
    console.log('Profile updated successfully');
    res.redirect('/profile?success=Perfil atualizado com sucesso!');
    
  } catch (error) {
    console.error('Error updating profile:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao atualizar perfil';
    res.redirect(`/profile?error=${encodeURIComponent(errorMessage)}`);
  }
});

router.post('/profile/change-password', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.user.id;

    console.log(`Changing password for user ${userId}`);
      const response = await axios.post(`${backendUrl}/api/users/${userId}/change-password`, req.body, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`,
        'Content-Type': 'application/json'
      }
    });
    
    console.log('Password changed successfully');
    res.redirect('/profile?success=Password alterada com sucesso!');
    
  } catch (error) {
    console.error('Error changing password:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao alterar password';
    res.redirect(`/profile?error=${encodeURIComponent(errorMessage)}`);
  }
});

router.get('/user/:username', async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const username = req.params.username;
    const currentUserId = req.user?.username;

    console.log(`Fetching public profile for user ${username}`);
    
    const userResponse = await axios.get(`${backendUrl}/api/users/public/${username}`);
    const userData = userResponse.data;
    const profileUser = userData.success ? userData.user : userData;

    const headers = {};
    if (req.cookies.token) {
      headers['Authorization'] = `Bearer ${req.cookies.token}`;
    }
    
    const postsResponse = await axios.get(`${backendUrl}/api/items/user/${profileUser.id}`, {
      headers: headers
    });
    const userPosts = postsResponse.data;

    const isOwnProfile = currentUserId === username;

    console.log('User profile data received:', { profileUser, userPosts: userPosts.length, isOwnProfile });

    res.render('user-profile', { 
      title: `Perfil de ${profileUser.nome} - Meu Eu Digital`,
      profileUser: profileUser,
      userPosts: userPosts,
      isOwnProfile: isOwnProfile
    });
    
  } catch (error) {
    console.error('Error fetching user profile:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar perfil do utilizador'
      }
    });
  }
});





// POST /api/items/:id/comments - Proxy to backend for adding comments
router.post('/api/items/:id/comments', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log(`Adding comment to item ${itemId}`);
    
    const response = await axios.post(`${backendUrl}/api/items/${itemId}/comments`, req.body, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`,
        'Content-Type': 'application/json'
      }
    });
    
    console.log('Comment added successfully');
    res.status(200).json(response.data);
    
  } catch (error) {
    console.error('Error adding comment:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao adicionar comentário';
    res.status(error.response?.status || 500).json({
      success: false,
      message: errorMessage
    });
  }
});

// PUT /api/items/:id/visibility - Proxy to backend for visibility toggle
router.put('/api/items/:id/visibility', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log(`Toggling visibility for item ${itemId}`);
    
    const response = await axios.put(`${backendUrl}/api/items/${itemId}/visibility`, req.body, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`,
        'Content-Type': 'application/json'
      }
    });
    
    console.log('Visibility toggled successfully');
    res.status(200).json(response.data);
    
  } catch (error) {
    console.error('Error toggling visibility:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao alterar visibilidade';
    res.status(error.response?.status || 500).json({
      success: false,
      message: errorMessage
    });
  }
});


router.get('/items', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.user.id;
    
    console.log(`Fetching items for user ${req.user.id}`);

    // Use the dedicated user items route that properly handles authentication
    const response = await axios.get(`${backendUrl}/api/items/user/${userId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    const items = response.data;
    console.log('Items received:', items.length);

    res.render('items', { 
      title: 'Meus Items - Meu Eu Digital',
      items: items
    });
      } catch (error) {
    console.error('Error fetching items:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar items'
      }
    });
  }
});

// DELETE /api/items/:id - Proxy to backend for deleting items
router.delete('/api/items/:id', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log(`Deleting item ${itemId}`);
    
    const response = await axios.delete(`${backendUrl}/api/items/${itemId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    console.log('Item deleted successfully');
    res.status(204).end();
    
  } catch (error) {
    console.error('Error deleting item:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao eliminar item';
    res.status(error.response?.status || 500).json({
      success: false,
      message: errorMessage
    });
  }
});

// GET /api/items/:id/export - Proxy to backend for exporting items as SIP
router.get('/api/items/:id/export', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log('=== FRONTEND PROXY EXPORT ===');
    console.log(`Exporting item ${itemId} as SIP`);
    console.log('Token from cookies:', req.cookies.token ? 'PRESENTE' : 'AUSENTE');
    console.log('Backend URL:', backendUrl);
    
    // Se o token não der, erro
    if (!req.cookies.token) {
      console.log('Sem token nos cookies');
      return res.status(401).json({
        success: false,
        message: 'Token de autenticação não encontrado'
      });
    }
    
    const response = await axios.get(`${backendUrl}/api/items/${itemId}/export`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      },
      responseType: 'stream'
    });
    
    console.log('Resposta do backend recebida:', response.status);
    
    res.setHeader('Content-Type', response.headers['content-type']);
    res.setHeader('Content-Disposition', response.headers['content-disposition']);
    if (response.headers['content-length']) {
      res.setHeader('Content-Length', response.headers['content-length']);
    }
    
    response.data.pipe(res);
    
  } catch (error) {
    console.error('Error exporting item:', error.message);
    if (error.response) {
      console.error('Error response data:', error.response.data);
      console.error('Error response status:', error.response.status);
    }
    const errorMessage = error.response?.data?.message || error.response?.data?.error || 'Erro ao exportar item';
    res.status(error.response?.status || 500).json({
      success: false,
      message: errorMessage
    });
  }
});

// GET /criar - Show create item page
router.get('/criar', requireAuth, function(req, res, next) {
  res.render('criar', { 
    title: 'Criar Post - Meu Eu Digital',
    success: req.query.success,
    error: req.query.error
  });
});

// GET /editar/:id - Show edit item page  
router.get('/editar/:id', requireAuth, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log(`Loading item ${itemId} for editing`);
    
    const response = await axios.get(`${backendUrl}/api/items/${itemId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    const item = response.data;
    
    if (item.submetidoPor._id !== req.user.id && req.user.tipo !== 'admin') {
      return res.status(403).render('error', {
        error: {
          status: 403,
          message: 'Não tem permissão para editar este item'
        }
      });
    }
    
    res.render('editar', { 
      title: 'Editar Post - Meu Eu Digital',
      item: item,
      success: req.query.success,
      error: req.query.error
    });
    
  } catch (error) {
    console.error('Error loading item for editing:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar item para edição'
      }
    });
  }
});

// GET /post/:id - Show individual post page
router.get('/post/:id', async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const itemId = req.params.id;
    
    console.log(`Loading post ${itemId}`);
    
    const headers = {};
    if (req.cookies.token) {
      headers['Authorization'] = `Bearer ${req.cookies.token}`;
    }
    
    const response = await axios.get(`${backendUrl}/api/items/${itemId}`, {
      headers: headers
    });
    
    const item = response.data;
    
    res.render('post', { 
      title: `${item.titulo} - Meu Eu Digital`,
      item: item
    });
    
  } catch (error) {
    console.error('Error loading post:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar post'
      }
    });
  }
});

// GET /gerir-users - Admin user management page
router.get('/gerir-users', requireAdmin, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    
    console.log('Admin accessing user management page');
    
    const response = await axios.get(`${backendUrl}/api/users`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    const users = response.data;
    console.log('Users retrieved for management:', users.length);    res.render('gerir-users', { 
      title: 'Gestão de Utilizadores - Meu Eu Digital',
      users: users,
      currentAdminId: req.user.id,
      success: req.query.success,
      error: req.query.error
    });
    
  } catch (error) {
    console.error('Error fetching users for management:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar utilizadores'
      }
    });
  }
});

// GET /gerir-users/editar/:id - Admin edit user page
router.get('/gerir-users/editar/:id', requireAdmin, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.params.id;
    
    console.log(`Admin editing user ${userId}`);
    
    const response = await axios.get(`${backendUrl}/api/users/${userId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    const userData = response.data;
    const user = userData.success ? userData.user : userData;
    
    res.render('editar-user', { 
      title: `Editar ${user.nome} - Gestão de Utilizadores`,
      editUser: user,
      success: req.query.success,
      error: req.query.error
    });
    
  } catch (error) {
    console.error('Error fetching user for editing:', error.message);
    res.render('error', {
      error: {
        status: error.response?.status || 500,
        message: 'Erro ao carregar utilizador para edição'
      }
    });
  }
});

// POST /gerir-users/editar/:id - Admin update user
router.post('/gerir-users/editar/:id', requireAdmin, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.params.id;
    
    console.log(`Admin updating user ${userId}`);
    
    const response = await axios.put(`${backendUrl}/api/users/${userId}`, req.body, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`,
        'Content-Type': 'application/json'
      }
    });
    
    console.log('User updated successfully by admin');
    res.redirect(`/gerir-users/editar/${userId}?success=Utilizador atualizado com sucesso!`);
    
  } catch (error) {
    console.error('Error updating user:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao atualizar utilizador';
    res.redirect(`/gerir-users/editar/${userId}?error=${encodeURIComponent(errorMessage)}`);
  }
});

// DELETE /api/users/:id - Admin delete user and all their posts
router.delete('/api/users/:id', requireAdmin, async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    const userId = req.params.id;
    
    console.log(`Admin deleting user ${userId} and all their posts`);
    
    const response = await axios.delete(`${backendUrl}/api/users/${userId}`, {
      headers: {
        'Authorization': `Bearer ${req.cookies.token}`
      }
    });
    
    console.log('User and all posts deleted successfully');
    res.status(204).end();
    
  } catch (error) {
    console.error('Error deleting user:', error.message);
    const errorMessage = error.response?.data?.message || 'Erro ao eliminar utilizador';
    res.status(error.response?.status || 500).json({
      success: false,
      message: errorMessage
    });
  }
});


const upload = multer({ dest: 'temp-uploads/' }); 

// POST /sip/upload - Handle SIP upload and forward to backend
router.post('/sip/upload', requireAuth, upload.single('sip'), async function(req, res, next) {
  try {
    const backendUrl = process.env.BACKEND_URL_AXIOS_GET;
    
    console.log('Frontend handling SIP upload for user:', req.user.username);
    
    if (!req.file) {
      return res.redirect('/sip/upload?error=Nenhum ficheiro foi selecionado');
    }
    
    const formData = new FormData();
    formData.append('sip', fs.createReadStream(req.file.path), {
      filename: req.file.originalname,
      contentType: req.file.mimetype
    });
    
    const response = await axios.post(`${backendUrl}/api/sip`, formData, {
      headers: {
        ...formData.getHeaders(),
        'Authorization': `Bearer ${req.cookies.token}`
      },
      maxRedirects: 0, 
      validateStatus: function (status) {
        return status >= 200 && status < 400; 
      }
    });

    if (fs.existsSync(req.file.path)) {
      fs.unlinkSync(req.file.path);
    }
    
    console.log('SIP upload successful');
    res.redirect('/sip/upload?success=SIP submetido com sucesso!');
    
  } catch (error) {
    console.error('Error in SIP upload:', error.message);
    
    if (req.file && fs.existsSync(req.file.path)) {
      fs.unlinkSync(req.file.path);
    }
    
    const errorMessage = error.response?.data?.message || error.message || 'Erro no upload do SIP';
    res.redirect(`/sip/upload?error=${encodeURIComponent(errorMessage)}`);
  }
});

module.exports = router;
