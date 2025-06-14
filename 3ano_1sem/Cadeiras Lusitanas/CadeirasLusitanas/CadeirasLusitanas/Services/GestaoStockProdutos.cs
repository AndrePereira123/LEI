using CadeirasLusitanas.Data;
using Microsoft.EntityFrameworkCore;
using CadeirasLusitanas.Models.Entities;

namespace CadeirasLusitanas.Services
{
    public class GestaoStockProdutos
    {
        private readonly CadeirasLusitanasContext? database;
        public GestaoStockProdutos(CadeirasLusitanasContext context)
        {
            database = context;
        }
        public async Task aumentaStock(int IDproduto, int quantidade, CadeirasLusitanasContext context)
        {
            try
            {
                var produto = await context.Product.FirstOrDefaultAsync(p => p.Id == IDproduto);
                if (produto == null)
                {
                    return;
                }
                produto.stock += quantidade;
                context.Update(produto);
                await context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                throw;
            }
        }
        public async Task<Boolean> diminuiStock(int IDproduto, int quantidade, CadeirasLusitanasContext context)
        {
            try
            {
                var produto = await context.Product.FirstOrDefaultAsync(p => p.Id == IDproduto);
                if (produto == null)
                {
                    return false;
                }
                if (produto.visible == 0)
                {
                    return false;
                }
                produto.stock = Math.Max(0, produto.stock - quantidade);
                context.Update(produto);
                await context.SaveChangesAsync();
                return true;
            }
            catch (DbUpdateConcurrencyException)
            {
                throw;
            }
        }
    }
}
