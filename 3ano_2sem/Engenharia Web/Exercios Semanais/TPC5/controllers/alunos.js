var Aluno = require("../models/aluno")

module.exports.list = () => {
    return Aluno
    .find()
    .sort({nome: 1})
    .exec()
}

module.exports.findById = id => {
    return Aluno
    .findOne({_id : id})
    .exec()
}

module.exports.insert = async (aluno) => {
    console.log('Incoming aluno:', aluno);

    // Check if a student with the same custom `id` already exists
    const existingAluno = await Aluno.findOne({ id: aluno.id }).exec();
    console.log('Existing aluno:', existingAluno);

    if (!existingAluno) {
        // Create a new student
        var newAluno = new Aluno(aluno);
        newAluno._id = aluno.id; // Set the custom ID
        console.log('New aluno:', newAluno);
        return newAluno.save();
    } else {
        throw new Error('A student with this ID already exists.');
    }
};

module.exports.update = (id, aluno) => {
    return Aluno.findByIdAndUpdate(id,aluno).exec()
}


module.exports.delete = id => {
    return Aluno.findByIdAndDelete(id).exec()
}

module.exports.inverteTpc = (id,idTpc) => {
    return Aluno
        .findOne({"_id" : id})
        .exec()
        .then(aluno => {
            var tpc = `tpc${idTpc}`
            if (aluno(tpc) != null) {
                aluno(tpc) = !aluno(tpc)
                }
            return Aluno 
                .findByIdAndUpdate(id, aluno)
                .exec()

            }

        )
}