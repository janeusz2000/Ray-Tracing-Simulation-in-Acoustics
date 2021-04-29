#!/bin/bash
echo "Validation Start at:" $(date) >> ./validationRaport.log

bazel build --config=_gcc validation
source ./venv/bin/activate

shopt -s nullglob
DIFFUSORS_ARRAY_PATH=( "validationDiffusors/*" )

for DIFFUSOR_PATH in $DIFFUSORS_ARRAY_PATH
do
   CURRENT_FILE=$(echo ${DIFFUSOR_PATH%.*} | cut -c21-)
   RAPORT_FILE="./validationResults/$CURRENT_FILE.json"
   REFERENCE_FILE="./validationResults/Reference/$CURRENT_FILE.json"

   rm $RAPORT_FILE
   touch $RAPORT_FILE

   bazel-bin/validation $RAPORT_FILE ./$DIFFUSOR_PATH
   python3 ./validationTools/compareResutlsToReference.py $REFERENCE_FILE $RAPORT_FILE
done

deactivate