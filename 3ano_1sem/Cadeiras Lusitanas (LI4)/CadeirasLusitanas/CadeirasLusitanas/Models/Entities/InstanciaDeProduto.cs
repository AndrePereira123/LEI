using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace CadeirasLusitanas.Models.Entities
{
    public class InstanciaDeProduto
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        
        [ForeignKey("Product")]
        public required int ProductID { get; set; }

        [ForeignKey("User")]
        [StringLength(45)]
        public required string UserEmail { get; set; }


        [Column("Estado")]
        public required char Estado { get; set; }

        [Column("Progresso")]
        public required int Progresso { get; set; }

        [Column("Tempo")]
        public float Tempo { get; set; }

    }
}
