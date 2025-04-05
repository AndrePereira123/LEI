using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.ViewModels
{
    public class ProductCreateViewModel
    {
        
        public required string nome { get; set; }

        public required string descricao { get; set; }

        public float peso { get; set; }

        public float custo { get; set; }


        public int? rodaEscolhida { get; set; }

        public int? pistaoEscolhido { get; set; }

        public int? encostoEscolhido { get; set; }

        public int? bracoDirEscolhido { get; set; }

        public int? bracoEsqEscolhido { get; set; }

        public int? almofadaEscolhida { get; set; }

        public int? numero_rodas { get; set; }
    }
}
