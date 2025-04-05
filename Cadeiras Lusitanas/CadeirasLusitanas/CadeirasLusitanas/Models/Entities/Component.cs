using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.Entities
{

    public class Component
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Column("Nome")]
        [Required(ErrorMessage = "O campo Nome é obrigatório.")]
        [StringLength(45)]
        public required string nome { get; set; }

        [Column("Descrição")]
        [Required(ErrorMessage = "O campo Descrição é obrigatório.")]
        [StringLength(200)]
        public required string descricao { get; set; }

        [Column("Peso")]
        [Required(ErrorMessage = "O campo Peso é obrigatório.")]

        public required float peso { get; set; }

        [Column("Tipo")]
        [StringLength(45)]
        public required string tipo { get; set; }

        [Column("Custo")]
        [Required(ErrorMessage = "O campo Custo é obrigatório.")]

        public required float custo { get; set; }

        [Column("Stock")]
        public int stock { get; set; }

        [Column("Tempo")]
        [RegularExpression(@"^\d+-\d+$", ErrorMessage = "O campo Tempo deve seguir o formato 'número-número', como '15-20'.")]
        [StringLength(10)]
        [Required(ErrorMessage = "O campo Tempo é obrigatório.")]
        public required string? tempo { get; set; }

        public ICollection<Product> Products { get; set; }

        [Column("Visível")]
        public int visible { get; set; }


    }

}
