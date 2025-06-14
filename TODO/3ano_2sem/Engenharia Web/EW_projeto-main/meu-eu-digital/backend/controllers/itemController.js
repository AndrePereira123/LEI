const Item = require('../models/Item');

// Listar todos os itens
module.exports.getItems = () => {
    return Item
            .find()
            .exec();
};

// Obter item por ID
module.exports.getItemById = id => {
    return Item
            .findById(id)
            .exec();
};

// Criar novo item
module.exports.createItem = itemData => {
    const item = new Item(itemData);
    return item.save();
};

// Atualizar item por ID
module.exports.updateItem = (id, itemData) => {
    return Item
            .findByIdAndUpdate(id, itemData, { new: true })
            .exec();
};

// Remover item por ID
module.exports.deleteItem = id => {
    return Item
            .findByIdAndDelete(id)
            .exec();
};

// Listar itens por classificador
module.exports.getItemsByClassificador = classificador => {
    return Item
            .find({ classificadores: classificador })
            .exec();
};

// Listar itens por visibilidade (publico/privado)
module.exports.getItemsByVisibilidade = visibilidade => {
    return Item
            .find({ visibilidade: visibilidade })
            .exec();
};

// Adicionar estes métodos ao controller
exports.getHomepageData = async (filtro = null, userInfo = null) => {
  let query = {};
  
  // Se o user for admin, mostrar todos os posts; caso contrário, apenas públicos
  if (userInfo && userInfo.tipo === 'admin') {
    query = {};
  } else {
    query = { visibilidade: 'publico' };
  }
  
  if (filtro) {
    query.classificadores = { $in: [filtro] };
  }
  
  const recentItems = await Item.find(query)
    .sort({ dataSubmissao: -1 })
    .populate('submetidoPor', 'username')
    .select('titulo dataSubmissao submetidoPor ficheiros comentarios classificadores visibilidade descricao');
  
  return recentItems;
};

// Pesquisa itens atraves de filtros
exports.searchItems = async (query = '', filtro = null) => {
  const searchQuery = { visibilidade: 'publico' };
  
  if (query && query.trim()) {
    searchQuery.$or = [
      { titulo: { $regex: query.trim(), $options: 'i' } },
      { descricao: { $regex: query.trim(), $options: 'i' } },
      { classificadores: { $in: [new RegExp(query.trim(), 'i')] } }
    ];
  }
  
  if (filtro) {
    searchQuery.classificadores = { $in: [filtro] };
  }
  
  const results = await Item.find(searchQuery)
    .sort({ dataSubmissao: -1 })
    .populate('submetidoPor', 'username')
    .select('titulo dataSubmissao submetidoPor ficheiros comentarios classificadores visibilidade descricao');
  
  return results;
};

// get items by user ID
exports.getItemsByUserId = async (userId, requestingUser = null) => {
  try {
    let query = { submetidoPor: userId };
    

    const isOwner = requestingUser && requestingUser.id === userId;
    const isAdmin = requestingUser && requestingUser.tipo === 'admin';
    
    if (!isOwner && !isAdmin) {
      query.visibilidade = 'publico';
    }
    
    const userItems = await Item.find(query)
      .sort({ dataSubmissao: -1 })
      .populate('submetidoPor', 'username')
      .select('titulo dataSubmissao submetidoPor ficheiros comentarios classificadores visibilidade descricao');
    
    return userItems;
  } catch (error) {
    console.error('Error getting user items:', error);
    throw error;
  }
};