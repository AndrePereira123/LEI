using CadeirasLusitanas.Components;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.AspNetCore.Authentication.Cookies;
using CadeirasLusitanas.Data;
using CadeirasLusitanas.Services;
using CadeirasLusitanas.Components.Pages.GestaoProdutos;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddDbContextFactory<CadeirasLusitanasContext>(options =>
    options.UseSqlServer(builder.Configuration.GetConnectionString("CadeirasLusitanasContext") ?? throw new InvalidOperationException("Connection string 'CadeirasLusitanasContext' not found.")));

builder.Services.AddScoped<GestaoStockComponentes>(); 
builder.Services.AddScoped<GestaoStockProdutos>();
builder.Services.AddScoped<CriarInstanciaProduto>();
builder.Services.AddScoped<LinhaDeMontagem>();
builder.Services.AddScoped<LinhaDeMontagemMethods>();
builder.Services.AddScoped<PaginaInicialMethods>();



builder.Services.AddQuickGridEntityFrameworkAdapter();

builder.Services.AddDatabaseDeveloperPageExceptionFilter();

// Add services to the container.
builder.Services.AddRazorComponents()
    .AddInteractiveServerComponents();



builder.Services.AddAuthentication(CookieAuthenticationDefaults.AuthenticationScheme)
    .AddCookie(options =>
    {
        options.Cookie.Name = "auth_token";
        options.LoginPath = "/login";
        options.Cookie.MaxAge = TimeSpan.FromMinutes(90);
        options.AccessDeniedPath = "/access-denied";
    });
builder.Services.AddAuthorization();
builder.Services.AddCascadingAuthenticationState();


var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error", createScopeForErrors: true);
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
    app.UseMigrationsEndPoint();
}

using (var scope = app.Services.CreateScope())
{
    var context = scope.ServiceProvider.GetRequiredService<CadeirasLusitanasContext>();
    DbInitializer.Initialize(context);
}

app.UseHttpsRedirection();

app.UseStaticFiles();
app.UseAuthentication();
app.UseAntiforgery();
app.UseAuthorization();

app.MapRazorComponents<App>()
    .AddInteractiveServerRenderMode();

app.Run();
