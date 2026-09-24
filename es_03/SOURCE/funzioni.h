#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "random.h"
#include <algorithm>

#pragma once

using namespace std;

double get_max(double a, double b)
{
   return (a > b) ? a : b;
}

// Black and Scholes analytical function for Call and Put
void Black_Scholes_Direct(vector<double> &C, vector<double> &P, Random rnd, int M)
{
   int S_0 = 100;         // asset_price_0
   double T_delivery = 1; // delivery_time
   double K = 100;        // Strike_price
   double r = 0.1;        // risk-free interest rate
   double sigma = 0.25;   // volatility

   // variables for calculations
   double S_t;
   double W;

   for (int i = 0; i < M; i++)
   {
      W = rnd.Gauss(0, T_delivery);

      S_t = S_0 * exp((r - pow(sigma, 2) / 2.0) * T_delivery + sigma * W); // Price at time

      // Call analytical
      C.push_back(exp(-r*T_delivery) * fmax(0, S_t - K));

      // Put analytical
      P.push_back(exp(-r*T_delivery) * fmax(0, K - S_t));
   }
   rnd.SaveSeed();
}

void Black_Scholes_Step_by_Step(vector<double> &C, vector<double> &P, Random rnd, int M)
{
   int S_0 = 100;         // asset_price_0
   double T_delivery = 1; // delivery_time
   double K = 100;        // Strike_price
   double r = 0.1;        // risk-free interest rate
   double sigma = 0.25;   // volatility

   // variables for calculations
   double S_t;
   double W;

   // size of step by step calculation
   const double h = 0.01;

   for (int i = 0; i < M; i++)
   {
      if (i <= 1000)
      {
         ofstream out;
         // Step by Step function
         S_t = S_0;
         string nomefile = "Traiettorie" + to_string(i);
         out.open("../DATA/" + nomefile + ".dat");
         for (int j = 0; j < 100; j++)
         {
            W = rnd.Gauss(0, T_delivery);
            S_t *= exp((r - pow(sigma, 2) / 2.0) * h + sigma * W * sqrt(h)); // Price at time
            if (j <= 50)
            {
               out << " " << j * h << "\t" << S_t << endl;
            }
         }
         out.close();
      }
      else
      {
         // Step by Step function
         S_t = S_0;
         for (int j = 0; j < 100; j++)
         {
            W = rnd.Gauss(0, T_delivery);
            S_t *= exp((r - pow(sigma, 2) / 2.0) * h + sigma * W * sqrt(h)); // Price at time
         }
      }

      // Call
      C.push_back(exp(-r) * fmax(0, S_t - K));

      // Put
      P.push_back(exp(-r) * fmax(0, K - S_t));
   }
   rnd.SaveSeed();
}
