using CadeirasLusitanas.Models.Entities;

namespace CadeirasLusitanas.Services
{
    public class CriarInstanciaProduto
    {

        public InstanciaDeProduto CriarInstancia(int id,string email)
        {
            var instancia = new InstanciaDeProduto
            {
                ProductID = id,
                UserEmail = email,
                Estado = 'E',
                Progresso = 0
            };
            return instancia;
        }

    }
}
