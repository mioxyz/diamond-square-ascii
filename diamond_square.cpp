#include <array>
#include <iostream>
#include <cmath>

#define NS_CANVAS_DATA_DIM 128
#define NS_CANVAS_DATA_SZ NS_CANVAS_DATA_DIM*NS_CANVAS_DATA_DIM + 32

#define _DEBUG_PRINT

namespace Canvas {
   const uint dim = NS_CANVAS_DATA_DIM; // for convenience

   std::array<uint, NS_CANVAS_DATA_SZ> data;

   uint data_get(uint x, uint y) {
      return Canvas::data[x+NS_CANVAS_DATA_DIM*y];
   }

   void data_set(uint x, uint y, uint newValue) {
#ifdef DEBUG_PRINT
      std::cout << std::endl << "data_set("<<x<<", "<<y<<", "<<newValue<<") ";
#endif
      Canvas::data[x + y*NS_CANVAS_DATA_DIM] = newValue;
   }

   void print() {
      std::cout << std::endl;
      for(int x = 0; x < dim; ++x) {
         for(int y = 0; y < dim; ++y) {
            // indeed, a map would be better
            if(Canvas::data_get(x,y) < 20) {
               std::cout << ".";
            }else if(Canvas::data_get(x,y) < 40) {
               std::cout << ",";
            }else if(Canvas::data_get(x,y) < 60) {
               std::cout << "'";
            }else if(Canvas::data_get(x,y) < 80) {
               std::cout << "\"";
            }else if(Canvas::data_get(x,y) < 100) {
               std::cout << ":";
            }else if(Canvas::data_get(x,y) < 120) {
               std::cout << ";";
            }else if(Canvas::data_get(x,y) < 140) {
                std::cout << "+";
            }else if(Canvas::data_get(x,y) < 160) {
               std::cout << "=";
            }else if(Canvas::data_get(x,y) < 180) {
               std::cout << "%";
            }else if(Canvas::data_get(x,y) < 200) {
               std::cout << "$";
            }else if(Canvas::data_get(x,y) < 220) {
               std::cout << "&";
            }else if(Canvas::data_get(x,y) < 240) {
               std::cout << "#";
            } else {
               std::cout << "@";
            }
         }
         std::cout << std::endl;
      }
   }

}; // namespace Canvas


namespace ReliefGenerator {
   /* for an integer x find the smallest number 2^n which is larger or equal to
   x and return n;
   I don't think this can overflow, assuming x is uint. */
   uint getExponentOf2PowN(uint x)
   {
      if(0 == x) return 0;
      uint n = 1;
      while(n < x) n *= 2;
      return n;
   }

   /* populates Canvas::data with nice diamond square cloud thing.
   Not sure if we should actually use this in worldgen */
   void diamondSquare()
   {
      // I bet there is a way to incorporate this in the first iteration,
      // but I don't have the head for this right now.

      Canvas::data_set(      0      ,       0      , rand()%280);
      Canvas::data_set(      0      , Canvas::dim  , rand()%280);
      Canvas::data_set(Canvas::dim  ,       0      , rand()%280);
      Canvas::data_set(Canvas::dim  , Canvas::dim  , rand()%280);
      Canvas::data_set(Canvas::dim/2, Canvas::dim/2, rand()%280);

      Canvas::data_set(Canvas::dim/2,      0       , rand()%280);
      Canvas::data_set(     0       , Canvas::dim/2, rand()%280);
      Canvas::data_set(Canvas::dim/2, Canvas::dim  , rand()%280);
      Canvas::data_set(Canvas::dim  , Canvas::dim/2, rand()%280);

      auto N = ReliefGenerator::getExponentOf2PowN(Canvas::dim);
      for(uint n = N; n > 1; n = n / 2)
      {
         uint nu = n/2;

         for(uint y = 0; y < N; y += n) {
            for(uint x = 0; x < N; x += n) {
#ifdef DEBUG_PRINT
               std::cout << std::endl << N << ";" << n << ";" << x << ";" << y << ";";
#endif
               Canvas::data_set( x+nu,y,
                  (rand()%12) + ((
                     Canvas::data_get(x,y)
                   + Canvas::data_get(x+n,y+n)
                  ) / 2)
               );

               Canvas::data_set( x,y+nu,
                  (rand()%12) + ((
                     Canvas::data_get(x,y)
                   + Canvas::data_get(x,y+n)
                  ) / 2)
               );

               Canvas::data_set( x+n,y+nu,
                  (rand()%12) + ((
                     Canvas::data_get(x+n,y)
                   + Canvas::data_get(x+n,y+n)
                  ) / 2)
               );

               Canvas::data_set( x+nu,y+n,
                  (rand()%12) + ((
                     Canvas::data_get(x,y+n)
                   + Canvas::data_get(x+n,y+n)
                  ) / 2)
               );

               Canvas::data_set( x+nu,y+nu,
                  (rand()%12) + ((
                     Canvas::data_get(x+nu,y   )
                   + Canvas::data_get(x   ,y+nu)
                   + Canvas::data_get(x+n ,y+nu)
                   + Canvas::data_get(x+nu,y+n )
                  ) / 4)
               ); //center
            }
         }
      }
      return;
   }
} // namespace ReliefGenerator


int main(int argc, char** argv) {

   if(argc == 2) {
      int k = strtol(argv[1], nullptr, 0);
      while(k-- > 0) rand();
   }

   ReliefGenerator::diamondSquare();
   Canvas::print();

   return 1;
}
