const mongoose = require("mongoose");

const livroSchema = new mongoose.Schema({
    _id: { type: String, required: true },
    title: { type: String, required: true },
    series: { type: String, default: null },
    author: { type: [String], required: true },
    rating: { type: String, default: null },
    description: { type: String, default: null },
    language: { type: String, default: null },
    isbn: { type: String, default: null },
    genres: { type: [String], default: [] },
    characters: { type: [String], default: [] },
    bookFormat: { type: String, default: null },
    edition: { type: String, default: null },
    pages: { type: String, default: null },
    publisher: { type: String, default: null },
    publishDate: { type: String, default: null },
    firstPublishDate: { type: String, default: null },
    awards: { type: [String], default: [] },
    numRatings: { type: String, default: null },
    ratingsByStars: { type: [String], default: [] },
    likedPercent: { type: String, default: null },
    setting: { type: [String], default: [] },
    coverImg: { type: String, default: null },
    bbeScore: { type: String, default: null },
    bbeVotes: { type: String, default: null },
    price: { type: String, default: null }
}, { versionKey: false });

module.exports = mongoose.model("Livro", livroSchema);