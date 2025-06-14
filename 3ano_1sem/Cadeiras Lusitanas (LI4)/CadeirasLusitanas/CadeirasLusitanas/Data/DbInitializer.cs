using CadeirasLusitanas.Data;
using CadeirasLusitanas.Models.Entities;
using Microsoft.EntityFrameworkCore;

public static class DbInitializer
{
    public static void Initialize(CadeirasLusitanasContext context)
    {
        context.Database.Migrate();

    
        // Adiciona users iniciais
        var users = new List<User>
        {
            new User { email = "admin@gmail.com", password = "pass", type = "ADMIN" },
            new User { email = "user@gmail.com", password = "pass", type = "USER" },
            new User { email = "user2@gmail.com", password = "pass2", type = "USER" }
        };

        var components = new List<Component>
        {
            new Component {nome = "Roda-default", descricao = "É uma roda default", peso =1,tipo ="RODA",custo =5,tempo ="3-6",visible=1},
            new Component {nome = "Pistão-default", descricao = "É um pistão default", peso =5,tipo ="PISTÃO",custo =15,tempo ="3-5",visible=1},
            new Component {nome = "Braço-d-default", descricao = "É um braço (direito) default", peso =2,tipo ="BRAÇO_DIREITO",custo =10,tempo ="2-3",visible=1},
            new Component {nome = "Braço-e-default", descricao = "É um braço (esquerdo) default", peso =2,tipo ="BRAÇO_ESQUERDO",custo =10,tempo ="2-3",visible=1},
            new Component {nome = "Encosto-default", descricao = "É um encosto default", peso =3,tipo ="ENCOSTO",custo =15,tempo ="4-5",visible=1},
            new Component {nome = "Almofada-default", descricao = "É uma almofada default", peso =1,tipo ="ALMOFADA",custo =4,tempo ="1-2",visible=1},
            new Component {nome = "Almofada-default2", descricao = "É uma almofada default", peso =1,tipo ="ALMOFADA",custo =4,tempo ="1-2",visible=1}
        };

        foreach (var user in users)
        {
            if (!context.User.Any(u => u.email == user.email))
            {
                context.User.Add(user);
            }
        }
        foreach (var comp in components)
        {
            if (!context.Component.Any(c => c.nome == c.nome))
            {
                context.Component.Add(comp);
            }
        }

        context.SaveChanges();


    }
}
