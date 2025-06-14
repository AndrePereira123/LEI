using CadeirasLusitanas.Data;
using CadeirasLusitanas.Models.Entities;
using Microsoft.EntityFrameworkCore;
using System;

namespace CadeirasLusitanas.Services
{

    public class LinhaDeMontagemMethods
    {
        private static Queue<InstanciaDeProduto> produtos_em_espera { get; set; } = new Queue<InstanciaDeProduto>();
        private static InstanciaDeProduto[]? fases { get; set; } = new InstanciaDeProduto[3];
        Random random = new Random();

        public float MediaTProduçao(int id,CadeirasLusitanasContext context)
        {
            var instancia_products = context.InstanciaDeProduto.Where(p => p.ProductID == id).ToList();
            var produzidos = 0;
            float tempo_total = 0;

            foreach (var instancia in instancia_products)
            {
                if (instancia.Estado == 'C')
                {
                    produzidos++;
                    tempo_total += instancia.Tempo;
                }
            }

            if (produzidos == 0) return 0;
            return (tempo_total/produzidos);
        }
        public IEnumerable<InstanciaDeProduto> ProdutosEmEspera
        {
            get
            {
                lock (produtos_em_espera)
                {
                    return produtos_em_espera.ToList();
                }
            }
        }

        public InstanciaDeProduto[] Fases
        {
            get
            {
                lock (fases)
                {
                    return (InstanciaDeProduto[])fases.Clone();
                }
            }
        }


        public (int, int) GetFaseTime(Product product, string tipo1, string tipo2)
        {
            var component_tipo1 = product.Components.FirstOrDefault(c => c.tipo == tipo1);
            var component_tipo2 = product.Components.FirstOrDefault(c => c.tipo == tipo2);

            var partes_1 = component_tipo1.tempo.Split('-');
            var partes_2 = component_tipo2.tempo.Split('-');

            if (component_tipo1.tipo == "RODA")
            {
                partes_1[0] = (int.Parse(partes_1[0]) * product.numeroRodas).ToString();
                partes_1[1] = (int.Parse(partes_1[1]) * product.numeroRodas).ToString();
            }

            if (component_tipo2.tipo == "RODA")
            {
                partes_2[0] = (int.Parse(partes_2[0]) * product.numeroRodas).ToString();
                partes_2[1] = (int.Parse(partes_2[1]) * product.numeroRodas).ToString();
            }

            var min_time1 = int.Parse(partes_1[0]);
            var max_time1 = int.Parse(partes_1[1]);
            var time1 = random.Next(min_time1, max_time1 + 1); //+1 para incluir o valor máximo

            var min_time2 = int.Parse(partes_2[0]);
            var max_time2 = int.Parse(partes_2[1]);
            var time2 = random.Next(min_time2, max_time2 + 1); //+1 para incluir o valor máximo

            return (time1, time2);
        }


        public void InsereNaLinhaDeEspera(int id, string email)
        {
            var instancia = new InstanciaDeProduto
            {
                ProductID = id,
                UserEmail = email,
                Estado = 'E',
                Progresso = 0
            };

            lock (produtos_em_espera)
            {
                produtos_em_espera.Enqueue(instancia);
                Monitor.PulseAll(produtos_em_espera);
            }
        }

        public async Task ProcessarFila(CadeirasLusitanasContext context)
        {
            foreach (var instance in produtos_em_espera)
            {
                context.Add(instance);
                await context.SaveChangesAsync();
            }

            InstanciaDeProduto instancia = null;
            while (true)
            {
                lock (fases)
                {
                    while (fases[0] != null)
                    { Monitor.Wait(fases); }
                }
                lock (produtos_em_espera)
                {
                    while (produtos_em_espera.Count == 0)
                    {
                        Monitor.Wait(produtos_em_espera);
                    }

                    instancia = produtos_em_espera.Dequeue();
                }
                lock (fases)
                {
                    fases[0] = instancia;
                    Monitor.PulseAll(fases);
                }


                if (instancia != null)
                {
                    context.Update(instancia);
                    await context.SaveChangesAsync();
                }
            }
        }

        public async Task ProcessarFasesAsync(CadeirasLusitanasContext context,int fase)
        {
            while (true)
            {
                InstanciaDeProduto instancia = null;
                int progressoInicial = 0, progressoFinal = 0;
                string[] componentes = null;

                lock (fases)
                {
                    switch (fase)
                    {
                        case 2:
                            while (fases[0] == null)
                            { Monitor.Wait(fases); }
                            if (fases[0] != null)
                            {
                                instancia = fases[0];
                                instancia.Estado = 'P';
                                instancia.Tempo = 0;
                                progressoInicial = 0;
                                progressoFinal = 30;
                                componentes = new[] { "RODA", "PISTÃO" };
                            }
                            break;

                        case 4:
                            while (fases[1] == null)
                            { Monitor.Wait(fases); }
                            if (fases[1] != null)
                            {
                                instancia = fases[1];
                                progressoInicial = 30;
                                progressoFinal = 80;
                                componentes = new[] { "ALMOFADA", "ENCOSTO" };
                            }
                            break;

                        case 6:
                            while (fases[2] == null)
                            { Monitor.Wait(fases); }
                            if (fases[2] != null)
                            {
                                instancia = fases[2];
                                progressoInicial = 80;
                                progressoFinal = 100;
                                componentes = new[] { "BRAÇO_DIREITO", "BRAÇO_ESQUERDO" };
                            }
                            break;
                    }
                }

                int time1 = 0, time2 = 0;
                Product? product = null;

                try
                {
                    product = await context.Product
                                           .Include(p => p.Components)
                                           .FirstOrDefaultAsync(p => p.Id == instancia.ProductID);

                    (time1, time2) = GetFaseTime(product, componentes[0], componentes[1]);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Erro: {ex.Message}");
                }

                await Task.Delay(time1 * 1000); //Multiplica aqui para na tabela ficar guardado em segundos
                instancia.Tempo += time1;
                instancia.Progresso = progressoInicial + (progressoFinal - progressoInicial) / 2;
                context.Update(instancia);
                await context.SaveChangesAsync();


                await Task.Delay(time2 * 1000);
                instancia.Tempo += time2;
                instancia.Progresso = progressoFinal;
                context.Update(instancia);
                await context.SaveChangesAsync();


                if (fase == 6)
                {
                    instancia.Estado = 'C';
                    product.stock += 1;
                    context.Update(instancia);
                    await context.SaveChangesAsync();
                }


                lock (fases)
                {
                    if (fase == 4)
                    {
                        while (fases[2] != null)
                        {
                            Monitor.Wait(fases);
                        }
                        fases[2] = instancia;
                        fases[1] = null;
                        Monitor.PulseAll(fases);
                    }
                    else if (fase == 2)
                    {
                        while (fases[1] != null)
                        {
                            Monitor.Wait(fases);
                        }
                        fases[1] = instancia;
                        fases[0] = null;
                        Monitor.PulseAll(fases);
                    }
                    else if (fase == 6)
                    {
                        fases[2] = null;
                        Monitor.PulseAll(fases);
                    }
                }
            }
        }
    }
}
