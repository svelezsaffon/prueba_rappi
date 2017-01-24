/*
 * cube_sum.cpp
 * 
 * Copyright 2017 Santiago <santiago@santiago-XPS-L412Z>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <string>
#include <list>

#define X 0
#define Y 1
#define Z 2
#define X1 3
#define Y1 4
#define Z1 5

class fenwick_solver{

public:
    long long fenwick_sum(long long x,long long y,long long z,long long cube[101][101][101]){
        long long y_index,x_index,fenwick_sum=0;
        while (z>0) {
            x_index=x;
            while(x_index>0) {
                y_index=y;
                while(y_index>0) {
                    fenwick_sum += cube[x_index][y_index][z];
                    y_index-= (y_index & -y_index);

                }
                x_index -= (x_index & -x_index);
            }
            z -= (z & -z);

        }
        return fenwick_sum;
    }


    void fenwick_update(long long n,long long x,long long y,long long z,long long  val,long long cube[101][101][101]) {
        long long y1,x1;

        while(z <= n) {
            x1 = x;
            while(x1 <= n) {
                y1 = y;
                while(y1 <= n) {
                    cube[x1][y1][z] += val;

                    y1 += (y1 & -y1 );
                }
                x1 += (x1 & -x1);
            }
            z += (z & -z);
        }

    }

    std::list<long long > solve(){



        unsigned int cases;
        std::cin>>cases;

        std::list<long long> values;

        while(cases--){
            long long cube[101][101][101]={0};
            int m;
            int n;
            std::cin>>n;
            std::cin>>m;



            while(m--){
                std::string oper;
                std::cin>>oper;

                long long cord[6]={0};

                if(oper=="UPDATE"){
                    //This is just to update the cube, quite simple;
                    for(unsigned int i=0;i<4;i++){
                        std::cin>>cord[i];
                    }


                    long long aux1=fenwick_sum(cord[X],cord[Y],cord[Z],cube)-fenwick_sum(cord[X]-1,cord[Y],cord[Z],cube)-fenwick_sum(cord[X],cord[Y]-1,cord[Z],cube)+fenwick_sum(cord[X]-1,cord[Y]-1,cord[Z],cube);

                    long long aux2=fenwick_sum(cord[X],cord[Y],cord[Z]-1,cube)-fenwick_sum(cord[X]-1,cord[Y],cord[Z]-1,cube)-fenwick_sum(cord[X],cord[Y]-1,cord[Z]-1,cube)+fenwick_sum(cord[X]-1,cord[Y]-1,cord[Z]-1,cube);

                    fenwick_update(n,cord[X],cord[Y],cord[Z],cord[3]-(aux1-aux2),cube);

                }else{

                    for(unsigned int i=0;i<6;i++){
                        std::cin>>cord[i];
                    }



                    long long aux1=fenwick_sum(cord[X1],cord[Y1],cord[Z1],cube)-fenwick_sum(cord[X]-1,cord[Y1],cord[Z1],cube)-fenwick_sum(cord[X1],cord[Y]-1,cord[Z1],cube)+fenwick_sum(cord[X]-1,cord[Y]-1,cord[Z1],cube);

                    long long aux2=fenwick_sum(cord[X1],cord[Y1],cord[Z]-1,cube)-fenwick_sum(cord[X]-1,cord[Y1],cord[Z]-1,cube)-fenwick_sum(cord[X1],cord[Y]-1,cord[Z]-1,cube)+fenwick_sum(cord[X]-1,cord[Y]-1,cord[Z]-1,cube);

                    values.push_back(aux1-aux2);

                }
            }
        }


        return values;
    }


};


int main() {

    fenwick_solver solver;

    std::list<long long> values=solver.solve();

    std::list<long long>:: iterator i=values.begin();
    std::list<long long>:: iterator end=values.end();
    for(;i!=end;i++){
        std::cout<<(*i)<<std::endl;
    }

    return 0;
}
