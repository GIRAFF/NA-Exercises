{-# OPTIONS_GHC -Wno-tabs #-}

module MakeF where

import Data.Matrix
import Data.Vector

{-generate Hilbert-}
hilbert :: Int -> Matrix Double 
hilbert n = matrix n n $ \(x,y) -> 1/(fromIntegral (x+y-1))

{-generate vector-}
vec :: Int -> Matrix Double
vec n = colVector $ generate n $ \x -> fromIntegral (x+1)

makeF :: Int -> [Double]
makeF n = Data.Vector.toList . getCol 1 $ multStd (hilbert n) (vec n) 
