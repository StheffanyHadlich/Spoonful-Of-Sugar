using System;
using System.Collections.Generic;

namespace app2.Models
{
    public class Remedio
    {
        public Guid Id { get; set; } = Guid.NewGuid();
        public string Nome { get; set; } = "";
        public string Quantidade {get; set; } = "";
        public int Intervalo {get; set;} = 0;

        public List<Alarme> Alarmes = new List<Alarme>();
    }
}