using CadeirasLusitanas.Data;
using Microsoft.EntityFrameworkCore;
using CadeirasLusitanas.Models.Entities;
using Microsoft.AspNetCore.Components;


namespace CadeirasLusitanas.Services
{
    public class GestaoStockComponentes
    {
        private readonly CadeirasLusitanasContext? database;

        public GestaoStockComponentes(CadeirasLusitanasContext context)
        {
            database = context;
        }

        public async Task aumentaStock(int IDcomponente,int quantidade, CadeirasLusitanasContext context)
        {
            if(quantidade == 0)
            {
                return;
            }  
            
            try
            {
                var componente = await context.Component.FirstOrDefaultAsync(c => c.Id ==  IDcomponente);
                if (componente == null)
                {
                    return;
                }
                componente.stock += quantidade;
                context.Update(componente);
                await context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                throw;
            }
        }

        public async Task<(Boolean,Boolean)> diminuiStock(int IDcomponente, int quantidade, CadeirasLusitanasContext context)
        {
            /*if (quantidade != 0)
            {
                return (false,false);
            }*/

            try
            {
                var componente = await context.Component.FirstOrDefaultAsync(c => c.Id == IDcomponente);
                if(componente == null)
                {
                    return (true,false);
                }
                if(componente.visible == 0)
                {
                    return (true, false);
                }
                componente.stock = Math.Max(0, componente.stock - quantidade);
                context.Component.Update(componente);
                await context.SaveChangesAsync();
                return (true,true);
            }
            catch (DbUpdateConcurrencyException)
            {
                throw;
            }
        }
    }
}
