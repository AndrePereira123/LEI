const express = require('express');
const router = express.Router();
const multer = require('multer');
const AdmZip = require('adm-zip');
const fs = require('fs');
const path = require('path');
const Item = require('../controllers/itemController');
const User = require('../models/User');

const upload = multer({ dest: 'uploads/sip/' });

// POST /api/sip
router.post('/', upload.single('sip'), async (req, res) => {
  let tempFilePath = null;
  
  try {
    
    const authHeader = req.get('Authorization');

    if (!authHeader || !authHeader.startsWith('Bearer ')) {
      return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=Utilizador não autenticado`);
    }

    const token = authHeader.split(' ')[1];
    const jwt = require('jsonwebtoken');
    
    let utilizadorLogado;
    try {
      const payload = jwt.verify(token, process.env.SECRET_KEY);
      utilizadorLogado = await User.findById(payload.id);
      
      if (!utilizadorLogado) {
        return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=Utilizador não encontrado`);
      }
    } catch (jwtError) {
      return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=Token inválido`);
    }

    tempFilePath = req.file.path;
    
    
    const zip = new AdmZip(tempFilePath);
    const zipEntries = zip.getEntries();

    
    const manifestoEntry = zipEntries.find(e => e.entryName.endsWith('manifesto-SIP.json'));
    if (!manifestoEntry) {
      return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=Manifesto nao encontrado no ZIP`);
    }

    
    const manifesto = JSON.parse(manifestoEntry.getData().toString('utf8'));

    const ficheirosNoZip = zipEntries
      .filter(e => !e.isDirectory && !e.entryName.endsWith('manifesto-SIP.json'))
      .map(e => e.entryName);

    let ficheirosManifesto = [];
    if (manifesto.items && Array.isArray(manifesto.items)) {
        manifesto.items.forEach(item => {
            if (item.ficheiro) ficheirosManifesto.push(item.ficheiro);
            if (item.metadados) ficheirosManifesto.push(item.metadados);
        });
    }

    const ficheirosEmFalta = ficheirosManifesto.filter(f => !ficheirosNoZip.includes(f));    if (ficheirosEmFalta.length > 0) {
        return res.redirect( `${process.env.FRONTEND_URL}/sip/upload?error=Ficheiros em falta no ZIP: ${ficheirosEmFalta.join(', ')}`);
    }
    
    
    const dataSubmissao = new Date().toISOString();
    manifesto.dataSubmissao = dataSubmissao;
    
    
    manifesto.submetidoPor = utilizadorLogado._id.toString();

    
    for (const item of manifesto.items) {
        if (item.metadados) {
            const metadadosEntry = zipEntries.find(e => e.entryName === item.metadados);
            if (metadadosEntry) {
                let metadados = JSON.parse(metadadosEntry.getData().toString('utf8'));
                metadados.dataSubmissao = dataSubmissao;
                metadados.submetidoPor = utilizadorLogado._id.toString(); // Usar o utilizador logado
                metadadosEntry.setData(Buffer.from(JSON.stringify(metadados, null, 2), 'utf8'));
            }
        }
    }

    
    if (manifestoEntry) {
        manifestoEntry.setData(Buffer.from(JSON.stringify(manifesto, null, 2), 'utf8'));
    }

    
    const idRaw = utilizadorLogado._id.toString() + '-' + dataSubmissao;
   
    const id = idRaw.replace(/[:]/g, '-');
    const destPath = path.join('uploads', id);
    fs.mkdirSync(destPath, { recursive: true });
    zip.extractAllTo(destPath, true);

    
    const itemsCreated = [];
    const errors = [];

    for (const [index, item] of manifesto.items.entries()) {
        try {
            const metadadosPath = path.join(destPath, item.metadados);
            let metadados = {};
            
            if (fs.existsSync(metadadosPath)) {
                try {
                    metadados = JSON.parse(fs.readFileSync(metadadosPath, 'utf8'));
                } catch (parseError) {
                    throw new Error(`Erro ao fazer parse do ficheiro de metadados ${item.metadados}: ${parseError.message}`);
                }
            } else {
                throw new Error(`Ficheiro de metadados não encontrado: ${item.metadados}`);
            }

            // Validar campos obrigatórios
            if (!metadados.titulo) {
                throw new Error(`Título é obrigatório para o item ${index + 1}`);
            }

            // Criar o item na base de dados
            const novoItem = {
                titulo: metadados.titulo,
                tipoRecurso: metadados.tipoRecurso || 'documento',
                dataCriacao: metadados.dataCriacao || new Date().toISOString(),
                dataSubmissao: metadados.dataSubmissao,
                produtor: metadados.produtor || utilizadorLogado.nome,
                submetidoPor: utilizadorLogado._id.toString(),
                visibilidade: metadados.visibilidade || 'privado',
                descricao: metadados.descricao || '',
                classificadores: Array.isArray(metadados.classificadores) ? metadados.classificadores : [],
                ficheiros: Array.isArray(metadados.ficheiros) ? metadados.ficheiros : [],
                comentarios: Array.isArray(metadados.comentarios) ? metadados.comentarios : []
            };

            console.log(`Criando item ${index + 1}: ${novoItem.titulo}`);
            const createdItem = await Item.createItem(novoItem);
            
            if (!createdItem) {
                throw new Error(`Falha ao criar item na base de dados: ${novoItem.titulo}`);
            }
            
            itemsCreated.push(createdItem);
            console.log(`Item criado com sucesso: ${createdItem.titulo || createdItem._id}`);
            
        } catch (itemError) {
            console.error(`Erro ao criar item ${index + 1}:`, itemError.message);
            errors.push({
                itemIndex: index + 1,
                itemTitle: metadados?.titulo || `Item ${index + 1}`,
                error: itemError.message
            });
            
        }
    }

    // Verificar se houve erros na criação de itens
    if (errors.length > 0) {
        console.error(`Erros na criação de ${errors.length} item(s):`, errors);
        
        // Se nenhum item foi criado, é um erro total
        if (itemsCreated.length === 0) {
            return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=${encodeURIComponent(`Falha ao criar itens na base de dados: ${errors.map(e => e.error).join('; ')}`)}`);
        }
        
        // Se alguns itens foram criados, é um sucesso parcial
        const successMessage = `SIP processado com ${itemsCreated.length} item(s) criado(s). ${errors.length} erro(s): ${errors.map(e => `${e.itemTitle} - ${e.error}`).join('; ')}`;
        return res.redirect(`${process.env.FRONTEND_URL}/sip/upload?success=${encodeURIComponent(successMessage)}`);
    }

    console.log(`SIP processado com sucesso. ${itemsCreated.length} item(s) criado(s).`);
    res.redirect(process.env.FRONTEND_URL + "/sip/upload?success=SIP submetido com sucesso!");
  } catch (err) {
    console.error('Erro no upload SIP:', err);
    res.redirect(`${process.env.FRONTEND_URL}/sip/upload?error=${encodeURIComponent(err.message)}`);
  } finally {
    // Remove o ficheiro ZIP temporário
    if (tempFilePath && fs.existsSync(tempFilePath)) {
      try {
        fs.unlinkSync(tempFilePath);
      } catch (unlinkError) {
        console.error('Erro ao remover ficheiro temporário:', unlinkError);
      }
    }
  }
});

module.exports = router;