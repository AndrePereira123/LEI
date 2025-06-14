using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.ViewModels
{
    public class AbastecerComponenteViewModel
    {
        [Required(AllowEmptyStrings = false, ErrorMessage = "Por favor insira uma quantidade")]
        public int quantidade { get; set; }
    }
}
