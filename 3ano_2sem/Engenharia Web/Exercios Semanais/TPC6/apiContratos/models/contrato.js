const mongoose = require("mongoose");

const contratoSchema = new mongoose.Schema({
    _id: Number,
    nAnuncio: { type: String, default: null },
    tipoprocedimento: String,
    objectoContrato: String,
    dataPublicacao: String,
    dataCelebracaoContrato: String,
    precoContratual: Number,
    prazoExecucao: String,
    NIPC_entidade_comunicante: Number,
    entidade_comunicante: String,
    fundamentacao: String
}, {versionKey: false});

module.exports = mongoose.model("contrato", contratoSchema);
