import {Component, Inject} from '@angular/core';
import {Http} from '@angular/http';
import {NgModel} from '@angular/forms';

@Component({selector: 'remedio', templateUrl: './remedio.component.html'})
export class RemedioComponent {
    public remedios : any[];
    public nome : string;
    public quantidade : string;
    public horaInicio : string;
    public intervalo : number;
    public intervalos : any = [
        {
            name: "24h",
            value: 24
        }, {
            name: "12h",
            value: 12
        }, {
            name: "8h",
            value: 8
        }, {
            name: "6h",
            value: 6
        }, {
            name: "4h",
            value: 4
        }
    ];

    constructor(private http : Http, @Inject('BASE_URL')private baseUrl : string) {
        this
            .http
            .get(this.baseUrl + 'api/Remedio')
            .subscribe(result => {
                this.remedios = result.json()as any[];
            }, error => console.error(error));
    }

    Save() {
        if (this.intervalo == null || this.nome == null ||this.horaInicio ==null){
            alert("Todos os dados precisam ser preenchidos");
        }
         else {
            var value = {Nome:this.nome,Quantidade:this.quantidade,Intervalo: this.intervalo};
            this.http
                .post(this.baseUrl + 'api/Remedio', value)
                .subscribe(result => {
                    this.remedios.push(result.json())
                });

        }

    }

     public Remove(remedio : any) {
        this.http
            .delete(this.baseUrl + 'api/Remedio/' + remedio.id)
            .subscribe(result => {
                if (result.status === 204) {
                    let index = this.remedios.indexOf(remedio);
                    this.remedios.splice(index, 1);
                }
            });
    }


    setIntervalo(gap : any) {
        this.intervalo = gap;
    }

}
