#!/bin/bash
echo "Validation Start at:" $(date) >>./validationRaport.log

bazel build --config=_gcc validation
source ./venv/bin/activate

SOURCE_POWER=1000
NUM_OF_COLLECTORS=33
NUM_OF_RAYS_SQUARED=5
MAX_TRACKING=15

echo STARTING VALIDATION FOR:
echo - SOURCE POWER: $SOURCE_POWER
echo - NUM_OF_COLLECTORS: $NUM_OF_COLLECTORS
echo - NUM_OF_RAYS_SQUARED: $NUM_OF_RAYS_SQUARED

python3 ./validationTools/loggingSimulationProperties.py $SOURCE_POWER $NUM_OF_COLLECTORS $NUM_OF_RAYS_SQUARED

shopt -s nullglob
DIFFUSORS_ARRAY_PATH=("validationDiffusors/*")

for DIFFUSOR_PATH in $DIFFUSORS_ARRAY_PATH; do

   CURRENT_FILE=$(echo ${DIFFUSOR_PATH%.*} | cut -c21-)

   RAPORT_LINE_FILE=""
   REFERENCE_FILE_LINE="./validationResults/Reference/line/$CURRENT_FILE.json"
   rm $RAPORT_LINE_FILE
   touch $RAPORT_LINE_FILE

   if [[ $CURRENT_FILE == "*2D*" ]]; then
      bazel-bin/validation "xyAxis" $RAPORT_LINE_FILE ./$DIFFUSOR_PATH $SOURCE_POWER $NUM_OF_COLLECTORS $NUM_OF_RAYS_SQUARED $MAX_TRACKING
      python3 ./validationTools/compareResutlsToReference.py $REFERENCE_FILE_LINE $RAPORT_FILE_LINE "Line"
   else
      bazel-bin/validation "xAxis" $RAPORT_LINE_FILE "./$DIFFUSOR_PATH" $SOURCE_POWER $NUM_OF_COLLECTORS $NUM_OF_RAYS_SQUARED $MAX_TRACKING
      python3 ./validationTools/compareResutlsToReference.py $REFERENCE_FILE_LINE $RAPORT_FILE_LINE "Line"
   fi

   RAPORT_SURFACE_FILE="./ValidationResults/RayTraced/surface/$CURRENT_FILE.json"
   REFERENCE_FILE_SURFACE="./validationResults/Reference/surface/$CURRENT_FILE.json"

   rm $RAPORT_SURFACE_FILE
   touch $RAPORT_SURFACE_FILE

   bazel-bin/validation "geoDome" $RAPORT_LINE_FILE "./$DIFFUSOR_PATH" $SOURCE_POWER $NUM_OF_COLLECTORS $NUM_OF_RAYS_SQUARED $MAX_TRACKING
   python3 ./validationTools/compareResutlsToReference.py $REFERENCE_FILE_SURFACE $RAPORT_FILE_SURFACE "Surface"
done

echo "Write short Validation Description: "
read -r DESC

echo "Final describtion: $DESC"
python3 ./validationTools/logValidation.py $DESC

deactivate
