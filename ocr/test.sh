#!/bin/bash

#./main "Training_Set/100 Arial.png"

search_dir=`ls ~/afs/SPE/OCR/ocr/Training_Set`                                                               
                                                                                    
for eachfile in $search_dir                                                         
do
    ./main "./Training_Set/$eachfile"
done    
