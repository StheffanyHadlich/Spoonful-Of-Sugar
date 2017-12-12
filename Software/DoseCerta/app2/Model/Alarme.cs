using System;

namespace app2.Models
{
    public class Alarme
    {
        public Guid Id { get; set; } = Guid.NewGuid();
       
        public int hora {get; set;} = 0;
        public int minuto {get; set;} = 0;
    }
}