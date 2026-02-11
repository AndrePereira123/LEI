var Livro = require("../models/livro");

module.exports.getLivros = () => {
    return Livro
        .find()
        .exec();
}

module.exports.getLivroById = id => {
    return Livro
        .findById(id)
        .exec();
}

module.exports.getLivrosByCharacter = function(character) {
    return Livro.find({ characters: { $regex: character, $options: 'i' } }).exec();
  };



module.exports.getLivrosByGenre = function(genre) {
  return Livro.find({ genres: { $regex: genre } }).exec();
};
 
module.exports.getLivrosByAuthor = author => {
  // Extract the string up until the first parenthesis or until it ends
  const extractedAuthor = author.split('(')[0].trim(); // Get the part before the first '(' and trim whitespace
  // Create a regex to match the extracted string
  const regex = new RegExp(`^${extractedAuthor}`, 'i'); // Match the string from the start, case-insensitive
  // Find all instances where the author matches the extracted string
  return Livro.find({ author: { $regex: regex } }).exec();
};

module.exports.getGenres = ()  => {
  return Livro.distinct("genres").exec(); 
};

module.exports.getCharacters = function() {
  return Livro.distinct('characters').then(characters => characters.sort());
};

module.exports.insert = function(livro) {
  return Livro.create(livro);
};

module.exports.delete = function(id) {
  return Livro.findByIdAndDelete(id).exec();
};

module.exports.update = function(id, livro) {
  return Livro.findByIdAndUpdate(id, livro, { new: true }).exec();
};



//------------------------------------------------------------------







module.exports.getEntidades = () => {
    return Livro
        .distinct("entidade_comunicante")
        .sort("entidade_comunicante")
        .exec();
}


module.exports.getTipos = () => {
    return Livro
        .distinct("tipoprocedimento")
        .exec();
}

//-----------------------------------------------------------



module.exports.getLivrosByEntidade = entidade => {
    return Livro
        .find({entidadecomunicante: entidade}) 
        .exec();
    }

    module.exports.getLivrosByTipo = tipo => {
        return Livro
            .find({ tipoprocedimento: tipo })
            .exec();
    }

    
    module.exports.update = (id, contr) => {
        return Livro
            .findByIdAndUpdate(id, contr, { new: true })
            .exec();
    }
    
    module.exports.insert = contr => {
        var contrToSave = new Livro(contr);
        return contrToSave.save()
    }
    
    module.exports.delete = id => {
        return Livro
            .findByIdAndDelete(id)
            .exec();
    }






