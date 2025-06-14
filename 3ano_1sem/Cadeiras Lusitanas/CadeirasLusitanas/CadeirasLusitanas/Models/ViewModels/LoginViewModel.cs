using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.ViewModels
{
    public class LoginViewModel
    {
        [Required(AllowEmptyStrings = false, ErrorMessage = "Por favor insira o seu email")]
        public string UserName { get; set; }
       
        [Required(AllowEmptyStrings = false, ErrorMessage = "Por favor insira a sua palavra-passe")]
        public string Password { get; set; }

    }
}
