using System;
using System.IO;
using app2.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

namespace app2.Data
{
    public class app2Context : DbContext
    {
        public DbSet<Remedio> Remedio { get; set; }
        public DbSet<Alarme> Alarmes { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=./Data/database.sqlite");
        }

        public static void InitDb(IServiceProvider serviceProvider)
        {
            const string path = "./Data/database.sqlite";
            if (File.Exists(path))
            {
                File.Delete(path);
            }
            var serviceScope = serviceProvider.GetRequiredService<IServiceScopeFactory>().CreateScope();
            var dataContext = serviceScope.ServiceProvider.GetRequiredService<app2Context>();
            dataContext.Database.EnsureCreated();
            dataContext.SaveChanges();
        }


    }
}