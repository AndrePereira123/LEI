const express = require('express');
const router = express.Router();
const Item = require('../controllers/itemController');
const ItemModel = require('../models/Item');
const User = require('../models/User');
const { validate } = require('../controllers/loginController');
const path = require('path');
const fs = require('fs'); 

// GET /api/items?classificador=...&visibilidade=...
router.get('/', function(req, res) {
  if (req.query.classificador) {
    Item.getItemsByClassificador(req.query.classificador)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).jsonp(error));
  }
  else if (req.query.visibilidade) {
    Item.getItemsByVisibilidade(req.query.visibilidade)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).jsonp(error));
  }
  else if(req.query.userId){
    Item.getItemsByUserId(req.query.userId)
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).jsonp(error));
  }
  else {
    Item.getItems()
      .then(data => res.status(200).jsonp(data))
      .catch(error => res.status(500).jsonp(error));
  }
});


const optionalAuth = (req, res, next) => {
  const authHeader = req.get('Authorization');
  
  if (!authHeader || !authHeader.startsWith('Bearer ')) {
    // sem autenticação, continuar como convidado
    req.user = null;
    return next();
  }

  const token = authHeader.split(' ')[1];
  const jwt = require('jsonwebtoken');

  jwt.verify(token, process.env.SECRET_KEY, (err, payload) => {
    if (err) {
      // Token inválido - continuar como convidado
      req.user = null;
    } else {
      req.user = payload;
    }
    next();
  });
};

router.get('/homepage-data/:filtro?', optionalAuth, async (req, res) => {
  try {
    console.log('Dados da homepage...');
    let recentItems = null;
    if (req.params.filtro) {
      recentItems = await Item.getHomepageData(req.params.filtro, req.user);
    }
    else {
      recentItems = await Item.getHomepageData(null, req.user);
    }    recentItems.forEach(item => {
      if (item.ficheiros && item.ficheiros.length > 0) {

        const idRaw = item.submetidoPor._id + '-' + item.dataSubmissao.toISOString();
        const id = idRaw.replace(/[:]/g, '-');
        
        try {
          const manifestoPath = path.join(__dirname, '../uploads', id, 'SIP/manifesto-SIP.json');
          console.log('Manifesto :', manifestoPath);

          if (fs.existsSync(manifestoPath)) {
            const manifestoContent = fs.readFileSync(manifestoPath, 'utf8');
            const manifestoData = JSON.parse(manifestoContent);

            
            item.ficheiros.forEach(f => {
              
              const itemManifestoCorrespondente = manifestoData.items.find(itemManifesto => {
                
                const nomeArquivoManifesto = path.basename(itemManifesto.ficheiro);
                return nomeArquivoManifesto === f.nomeOriginal;
              });

              if (itemManifestoCorrespondente) {
                const ficheiroPath = path.join(__dirname, '../uploads', id, itemManifestoCorrespondente.ficheiro);
                
                if (fs.existsSync(ficheiroPath)) {
                  const backendUrl = process.env.MY_URL;
                  f.caminho = `${backendUrl}/uploads/${id}/${itemManifestoCorrespondente.ficheiro}`;
                }
              }
            });
          }
        } catch (error) {
          console.error('Erro ao processar manifesto:', error);
        }
      }
    });
    
    res.status(200).json(recentItems);
    
  } catch (error) {
    console.error('Erro ao buscar dados da homepage:', error);
    res.status(500).json({ error: 'Erro interno do servidor' });
  }
});


// GET /api/items/search - Search items
router.get('/search', optionalAuth, async (req, res) => {
  try {
    console.log('Pesquisando items...');
    const { q, categoria } = req.query;
      let searchResults = await Item.searchItems(q, categoria);
    
    searchResults.forEach(item => {
      if (item.ficheiros && item.ficheiros.length > 0) {
        const idRaw = item.submetidoPor._id + '-' + item.dataSubmissao.toISOString();
        const id = idRaw.replace(/[:]/g, '-');
        
        try {
          const manifestoPath = path.join(__dirname, '../uploads', id, 'SIP/manifesto-SIP.json');
          
          if (fs.existsSync(manifestoPath)) {
            const manifestoContent = fs.readFileSync(manifestoPath, 'utf8');
            const manifestoData = JSON.parse(manifestoContent);
            
            item.ficheiros.forEach(f => {
              const itemManifestoCorrespondente = manifestoData.items.find(itemManifesto => {
                const nomeArquivoManifesto = path.basename(itemManifesto.ficheiro);
                return nomeArquivoManifesto === f.nomeOriginal;
              });

              if (itemManifestoCorrespondente) {
                const ficheiroPath = path.join(__dirname, '../uploads', id, itemManifestoCorrespondente.ficheiro);
                
                if (fs.existsSync(ficheiroPath)) {
                  const backendUrl = process.env.MY_URL;
                  f.caminho = `${backendUrl}/uploads/${id}/${itemManifestoCorrespondente.ficheiro}`;
                }
              }
            });
          }
        } catch (error) {
          console.error('Erro ao processar manifesto na pesquisa:', error);
        }
      }
    });
    
    res.status(200).json({
      success: true,
      query: q || '',
      categoria: categoria || '',
      results: searchResults,
      count: searchResults.length
    });
    
  } catch (error) {
    console.error('Erro ao pesquisar items:', error);
    res.status(500).json({ 
      success: false,
      error: 'Erro interno do servidor' 
    });
  }
});

// GET /api/items/user/:userId - Get items by user ID for profile page
router.get('/user/:userId', optionalAuth, async (req, res) => {
  try {
    const userId = req.params.userId;
    console.log(`Getting items for user ${userId}`);
    
    const userItems = await Item.getItemsByUserId(userId, req.user);
    
    userItems.forEach(item => {
      if (item.ficheiros && item.ficheiros.length > 0) {
        const idRaw = item.submetidoPor._id + '-' + item.dataSubmissao.toISOString();
        const id = idRaw.replace(/[:]/g, '-');
        
        try {
          const manifestoPath = path.join(__dirname, '../uploads', id, 'SIP/manifesto-SIP.json');
          
          if (fs.existsSync(manifestoPath)) {
            const manifestoContent = fs.readFileSync(manifestoPath, 'utf8');
            const manifestoData = JSON.parse(manifestoContent);
            
            item.ficheiros.forEach(f => {
              const itemManifestoCorrespondente = manifestoData.items.find(itemManifesto => {
                const nomeArquivoManifesto = path.basename(itemManifesto.ficheiro);
                return nomeArquivoManifesto === f.nomeOriginal;
              });

              if (itemManifestoCorrespondente) {
                const ficheiroPath = path.join(__dirname, '../uploads', id, itemManifestoCorrespondente.ficheiro);
                
                if (fs.existsSync(ficheiroPath)) {
                  const backendUrl = process.env.MY_URL;
                  f.caminho = `${backendUrl}/uploads/${id}/${itemManifestoCorrespondente.ficheiro}`;
                }
              }
            });
          }
        } catch (error) {
          console.error('Erro ao processar manifesto para perfil de usuário:', error);
        }
      }
    });
    
    res.status(200).json(userItems);
    
  } catch (error) {
    console.error('Erro ao buscar items do usuário:', error);
    res.status(500).json({ 
      success: false,
      error: 'Erro interno do servidor' 
    });
  }
});

// GET /api/items/:id
router.get('/:id', function(req, res) {
  Item.getItemById(req.params.id)
    .then(data => {
      if (data) res.status(200).jsonp(data);
      else res.status(404).jsonp({ error: 'Item não encontrado' });
    })
    .catch(error => res.status(500).jsonp(error));
});

// POST /api/items
router.post('/', function(req, res) {
  Item.createItem(req.body)
    .then(data => res.status(201).jsonp(data))
    .catch(error => res.status(500).jsonp(error));
});

// PUT /api/items/:id/visibility - Update item visibility (requires authentication)
router.put('/:id/visibility', validate, async function(req, res) {
  try {
    const itemId = req.params.id;
    const { visibilidade } = req.body;
    const userId = req.user.id;
    const userType = req.user.tipo;
    
    if (!visibilidade || !['publico', 'privado'].includes(visibilidade)) {
      return res.status(400).json({
        success: false,
        message: 'Visibilidade deve ser "publico" ou "privado"'
      });
    }

    const item = await ItemModel.findById(itemId);
    if (!item) {
      return res.status(404).json({
        success: false,
        message: 'Item não encontrado'
      });
    }
    
    const isOwner = item.submetidoPor.toString() === userId;
    const isAdmin = userType === 'admin';
    
    if (!isOwner && !isAdmin) {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para alterar este item'
      });
    }
    
    const updatedItem = await ItemModel.findByIdAndUpdate(
      itemId, 
      { visibilidade }, 
      { new: true }
    );
    
    res.status(200).json({
      success: true,
      message: 'Visibilidade alterada com sucesso',
      item: updatedItem
    });
    
  } catch (error) {
    console.error('Erro ao alterar visibilidade:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
});


// POST /api/items/:id/comments - Add comment to item (requires authentication)
router.post('/:id/comments', validate, async function(req, res) {
  try {
    const itemId = req.params.id;
    const {texto, autor_antigo} = req.body;
    const userId = req.user.id;

    const user = await User.findById(userId);
    autor = user ? user.username : null;
    
    if (!texto || texto.trim().length === 0) {
      return res.status(400).json({
        success: false,
        message: 'Texto do comentário é obrigatório'
      });
    }

    if (texto.trim().length > 500) {
      return res.status(400).json({
        success: false,
        message: 'Comentário não pode ter mais de 500 caracteres'
      });
    }

    const item = await ItemModel.findById(itemId);
    if (!item) {
      return res.status(404).json({
        success: false,
        message: 'Item não encontrado'
      });
    }
    
    const newComment = {
      texto: texto.trim(),
      autor: autor || req.user.username,
      data: new Date(),
      autorId: userId
    };
    
    const updatedItem = await ItemModel.findByIdAndUpdate(
      itemId,
      { $push: { comentarios: newComment } },
      { new: true }
    );
    
    res.status(200).json({
      success: true,
      message: 'Comentário adicionado com sucesso',
      comment: newComment,
      item: updatedItem
    });
    
  } catch (error) {
    console.error('Erro ao adicionar comentário:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
});

// PUT /api/items/:id
router.put('/:id', function(req, res) {
  Item.updateItem(req.params.id, req.body)
    .then(data => {
      if (data) res.status(200).jsonp(data);
      else res.status(404).jsonp({ error: 'Item não encontrado' });
    })
    .catch(error => res.status(500).jsonp(error));
});

// DELETE /api/items/:id
router.delete('/:id', validate, async function(req, res) {
  try {
    const itemId = req.params.id;
    const userId = req.user.id;
    const userType = req.user.tipo;
    
    const item = await ItemModel.findById(itemId);
    if (!item) {
      return res.status(404).json({
        success: false,
        message: 'Item não encontrado'
      });
    }
    
    const isOwner = item.submetidoPor.toString() === userId;
    const isAdmin = userType === 'admin';
    
    if (!isOwner && !isAdmin) {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para eliminar este item'
      });
    }
    
    const deletedItem = await Item.deleteItem(itemId);
    if (deletedItem) {
      res.status(204).end();
    } else {
      res.status(404).json({ 
        success: false,
        message: 'Item não encontrado' 
      });
    }
    
  } catch (error) {
    console.error('Erro ao eliminar item:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
});

// GET /api/items/:id/export - Export item as SIP (requires authentication)
router.get('/:id/export', validate, async function(req, res) {
  try {
    const itemId = req.params.id;
    const userId = req.user.id;
    const userType = req.user.tipo;
    
    const item = await ItemModel.findById(itemId).populate('submetidoPor', 'username');
    if (!item) {
      return res.status(404).json({
        success: false,
        message: 'Item não encontrado'
      });
    }
    
    const isOwner = item.submetidoPor._id.toString() === userId;
    const isAdmin = userType === 'admin';
    const isPublic = item.visibilidade === 'publico';
    
    if (!isOwner && !isAdmin && !isPublic) {
      return res.status(403).json({
        success: false,
        message: 'Não tem permissão para exportar este item'
      });
    }
    
    const AdmZip = require('adm-zip');
    const zip = new AdmZip();
    
    const sipData = {
      produtor: item.submetidoPor.username,
      submetidoPor: item.submetidoPor.username,
      dataCriacao: item.dataSubmissao.toISOString().split('T')[0],
      dataSubmissao: item.dataSubmissao.toISOString().split('T')[0],
      titulo: item.titulo,
      descricao: item.descricao || '',
      classificadores: item.classificadores || [],
      visibilidade: item.visibilidade,
      items: []
    };
    
    if (item.ficheiros && item.ficheiros.length > 0) {
      const idRaw = item.submetidoPor._id + '-' + item.dataSubmissao.toISOString();
      const uploadId = idRaw.replace(/[:]/g, '-');
      
      for (let i = 0; i < item.ficheiros.length; i++) {
        const ficheiro = item.ficheiros[i];
        const fileName = ficheiro.nomeOriginal;
        const fileBaseName = path.parse(fileName).name;
        const fileExt = path.parse(fileName).ext;
        
        const dataFolderPath = `SIP/data/${fileBaseName}/`;
        const filePath = `${dataFolderPath}${fileName}`;
        const metadataPath = `${dataFolderPath}${fileBaseName}.json`;
        
        const uploadPath = path.join(__dirname, '../uploads', uploadId);
        let actualFilePath = null;
        
        const manifestPath = path.join(uploadPath, 'SIP/manifesto-SIP.json');
        if (fs.existsSync(manifestPath)) {
          try {
            const manifestContent = fs.readFileSync(manifestPath, 'utf8');
            const manifestData = JSON.parse(manifestContent);
            
            const manifestItem = manifestData.items.find(item => {
              const manifestFileName = path.basename(item.ficheiro);
              return manifestFileName === fileName;
            });
            
            if (manifestItem) {
              actualFilePath = path.join(uploadPath, manifestItem.ficheiro);
            }
          } catch (error) {
            console.error('Error reading manifest:', error);
          }
        }
        
        if (actualFilePath && fs.existsSync(actualFilePath)) {
          const fileBuffer = fs.readFileSync(actualFilePath);
          zip.addFile(filePath, fileBuffer);
          
          const metadata = {
            titulo: item.titulo,
            descricao: item.descricao || '',
            ficheiro: fileName,
            tamanho: fileBuffer.length,
            tipo: fileExt.substring(1) || 'unknown',
            dataSubmissao: item.dataSubmissao.toISOString(),
            autor: item.submetidoPor.username,
            classificadores: item.classificadores || [],
            visibilidade: item.visibilidade
          };
          
          zip.addFile(metadataPath, Buffer.from(JSON.stringify(metadata, null, 2), 'utf8'));
          
          sipData.items.push({
            ficheiro: filePath,
            metadados: metadataPath
          });
        }
      }
    }
    
    zip.addFile('SIP/manifesto-SIP.json', Buffer.from(JSON.stringify(sipData, null, 2), 'utf8'));
    
    const zipBuffer = zip.toBuffer();
    
    const filename = `${item.titulo.replace(/[^a-zA-Z0-9]/g, '_')}_SIP.zip`;
    res.setHeader('Content-Type', 'application/zip');
    res.setHeader('Content-Disposition', `attachment; filename="${filename}"`);
    res.setHeader('Content-Length', zipBuffer.length);
    
    res.send(zipBuffer);
    
  } catch (error) {
    console.error('Erro ao exportar item como SIP:', error);
    res.status(500).json({
      success: false,
      message: 'Erro interno do servidor'
    });
  }
});



module.exports = router;