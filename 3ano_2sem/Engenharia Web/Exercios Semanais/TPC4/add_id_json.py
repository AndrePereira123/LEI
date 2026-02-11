import json


with open("dataset_cinema.json", "r", encoding="utf-8") as file:
    data = json.load(file)


    filmes = data["filmes"]
    max_id = 0

    for i, filme in enumerate(filmes):
        if "id" not in filme:
            max_id += 1  # Increment ID
            filme["id"] = max_id

        filmes[i] = {"id": filme["id"], **{k: v for k, v in filme.items() if k != "id"}}

    with open("dataset_cinema.json", "w", encoding="utf-8") as file:
        json.dump(data, file, indent=2, ensure_ascii=False)
