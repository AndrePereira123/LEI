using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using CadeirasLusitanas.Models.Entities;

namespace CadeirasLusitanas.Data
{
    public class CadeirasLusitanasContext : DbContext
    {
        public CadeirasLusitanasContext (DbContextOptions<CadeirasLusitanasContext> options)
            : base(options)
        {
        }

        public DbSet<CadeirasLusitanas.Models.Entities.User> User { get; set; } = default!;
        public DbSet<CadeirasLusitanas.Models.Entities.Component> Component { get; set; } = default!;
        public DbSet<CadeirasLusitanas.Models.Entities.Product> Product { get; set; } = default!;
        public DbSet<CadeirasLusitanas.Models.Entities.InstanciaDeProduto> InstanciaDeProduto { get; set; } = default!;



        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Product>()
                .HasMany(x => x.Components)
                .WithMany(y => y.Products)
                .UsingEntity(j => j.ToTable("ProductHasComponent"));

            base.OnModelCreating(modelBuilder);
        }
    }
}
