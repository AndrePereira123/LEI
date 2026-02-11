var Contrato = require("../models/contrato");

module.exports.getContratos = () => {
    return Contrato
        .find()
        .exec();
}

module.exports.getContratoById = id => {
    return Contrato
        .findById(id)
        .exec();
}

module.exports.getEntidades = () => {
    return Contrato
        .distinct("entidade_comunicante")
        .sort("entidade_comunicante")
        .exec();
}


module.exports.getTipos = () => {
    return Contrato
        .distinct("tipoprocedimento")
        .exec();
}




    module.exports.getContratosByEntidade = entidade => {    
        return Contrato
                .find({entidade_comunicante : entidade})
                .exec()
        }


//-----------------------------------------------------------

    module.exports.getContratosByTipo = tipo => {
        return Contrato
            .find({ tipoprocedimento: tipo })
            .exec();
    }

    module.exports.getContratosByNIPC = NIPC => {
        return Contrato
            .find({ NIPC_entidade_comunicante : NIPC })
            .exec();
    }

    
    module.exports.update = (id, contr) => {
        return Contrato
            .findByIdAndUpdate(id, contr, { new: true })
            .exec();
    }
    
    module.exports.insert = contr => {
        var contrToSave = new Contrato(contr);
        return contrToSave.save()
    }
    
    module.exports.delete = id => {
        return Contrato
            .findByIdAndDelete(id)
            .exec();
    }
    





