import json
import ast

# Caminho do ficheiro JSON original e do ficheiro formatado
input_file = "dataset.json"
output_file = "formatted_output.json"

# Ler o ficheiro JSON
with open(input_file, "r", encoding="utf-8") as file:
    data = json.load(file)

# Escrever o JSON formatado
with open(output_file, "w", encoding="utf-8") as file:
    json.dump(data, file, indent=4, ensure_ascii=False)

print(f"Ficheiro formatado guardado em {output_file}")


with open(output_file, "r", encoding="utf-8") as file:
    data = json.load(file)

for filme in data:
    filme["genres"] = ast.literal_eval(filme["genres"])
    filme["characters"] = ast.literal_eval(filme["characters"])
    filme["awards"] = ast.literal_eval(filme["awards"])
    filme["ratingsByStars"] = ast.literal_eval(filme["ratingsByStars"])
    filme["setting"] = ast.literal_eval(filme["setting"])

    # Change bookId to _id
    if "bookId" in filme:
        filme["_id"] = filme.pop("bookId")
    
    # Convert author to a list
    if "author" in filme and isinstance(filme["author"], str):
        filme["author"] = [author.strip() for author in filme["author"].split(",")]


with open(output_file, "w", encoding="utf-8") as file:
    json.dump(data, file, indent=4, ensure_ascii=False)