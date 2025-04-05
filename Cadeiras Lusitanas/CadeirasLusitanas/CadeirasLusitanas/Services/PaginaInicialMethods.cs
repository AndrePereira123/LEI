using CadeirasLusitanas.Models.Entities;
using CadeirasLusitanas.Data;

namespace CadeirasLusitanas.Services
{
    public class PaginaInicialMethods
    {
        public List<Product> LoadProducts(CadeirasLusitanasContext context)
        {
            List<Product> products = new List<Product>();
            var instancia_products = context.InstanciaDeProduto.ToList();
            var previous_product_id = -1;
            var produzidos = 0;
            Product product1 = null;

            foreach (var instancia in instancia_products)
            {
                if (previous_product_id != instancia.ProductID)
                {
                    if (product1 != null)
                    {
                        product1.stock = produzidos;
                        product1.custo *= produzidos;
                        products.Add(product1);
                    }

                    var product = context.Product.FirstOrDefault(p => p.Id == instancia.ProductID);
                    if (product == null)
                    {
                        continue;
                    }

                    product1 = new Product
                    {
                        Id = product.Id,
                        nome = product.nome,
                        descricao = product.descricao,
                        peso = product.peso,
                        custo = product.custo,
                        stock = 0
                    };

                    previous_product_id = instancia.ProductID;
                    produzidos = 0;
                }
                produzidos++;
            }

            if (product1 != null)
            {
                product1.stock = produzidos;
                product1.custo *= produzidos;
                products.Add(product1);
            }

            return products;
        }

        public List<Product> StatisticsForProducts(CadeirasLusitanasContext context)
        {
            List<Product> products = new List<Product>();
            var products_in_db = context.Product.ToList();

            foreach (var product in products_in_db)
            {
                var instancia_products = context.InstanciaDeProduto.Where(p => p.ProductID == product.Id).ToList();
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

                var temp_product = new Product
                {
                    Id = product.Id,
                    nome = product.nome,
                    descricao = product.descricao,
                    peso = tempo_total / produzidos, // Tempo médio
                    custo = product.custo * produzidos,
                    stock = produzidos
                };

                products.Add(temp_product);
            }
            return products;
        }

    }
}
