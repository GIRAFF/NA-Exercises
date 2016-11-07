{-# OPTIONS_GHC -Wno-tabs #-}

import MakeF

ia :: [Int]
ia = [0,0,1,3,6,10,15,21,28,36,45]

al :: Int -> [Int]
al 1 = [2]
al x = (al $! x-1) ++ take x [x+1,x+2..]

main :: IO ()
main = mapM_ print (map (\n -> (n,
			take (n+1) ia,
			take n (map (\x->1/x) [1,3..]),
			map (\x->1.0/(fromIntegral x)) (al $! n-1),
			makeF n)) [3..10])
