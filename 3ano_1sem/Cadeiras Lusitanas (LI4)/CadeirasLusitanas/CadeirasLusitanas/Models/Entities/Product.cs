using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.Entities
{
    public class Product
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Column("Nome")]
        [StringLength(45)]
        public required string nome { get; set; }

        [Column("Descrição")]
        [StringLength(200)]
        public required string descricao { get; set; }
       

        [Column("Peso")]
        public float peso { get; set; }

        [Column("Custo")]
        public  float custo { get; set; } 

        [Column("Stock")]
        public int stock { get; set; } 

        public ICollection<Component> Components { get; set; }

        [Column("Visível")]
        public int visible { get; set; }

        [Column("Número_Rodas")]
        public int numeroRodas { get; set; }

    }
}
