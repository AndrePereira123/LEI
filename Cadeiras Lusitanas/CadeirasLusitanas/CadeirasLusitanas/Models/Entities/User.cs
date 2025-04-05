using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;


namespace CadeirasLusitanas.Models.Entities
{
    public class User
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Column("email")]
        [StringLength(45)]
        public required string email { get; set; }

        [Column("password")]
        [StringLength(45)]
        public required string password { get; set; }

        [Column("role")]
        [StringLength(5)]
        public required string type { get; set; }
    }
}
