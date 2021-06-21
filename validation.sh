#!/bin/bash
echo "Validation Start at:" $(date) >>./validationRaport.log

bazel build --config=_gcc validation
source ./venv/bin/activate

SOURCE_POWER=1000
NUM_OF_COLLECTORS=40
NUM_OF_RAYS_SQUARED=100

echo STARTING VALIDATION FOR:
echo - SOURCE POWER: $SOURCE_POWER
echo - NUM_OF_COLLECTORS: $NUM_OF_COLLECTORS
echo - NUM_OF_RAYS_SQUARED: $NUM_OF_RAYS_SQUARED

python3 ./validationTools/loggingSimulationProperties.py $SOURCE_POWER $NUM_OF_COLLECTORS $NUM_OF_RAYS_SQUARED

shopt -s nullglob
DIFFUSORS_ARRAY_PATH=("validationDiffusors/*")

for DIFFUSOR_PATH in $DIFFUSORS_ARRAY_PATH; do
   CURRENT_FILE=$(echo ${DIFFUSOR_PATH%.*} | cut -c21-)
   RAPORT_FILE="./validationResults/$CURRENT_FILE.json"
   REFERENCE_FILE="./validationResults/Reference/$CURRENT_FILE.json"

   rm $RAPORT_FILE
   touch $RAPORT_FILE

   bazel-bin/validation $RAPORT_FILE ./$DIFFUSOR_PATH
   python3 ./validationTools/compareResutlsToReference.py $REFERENCE_FILE $RAPORT_FILE
done

echo "Write short Validation Description: "
read -r DESC

echo "Final describtion: $DESC"
python3 ./validationTools/logValidation.py $DESC

# deactivate
