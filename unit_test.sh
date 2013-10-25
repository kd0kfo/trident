#!/bin/sh

set -e

if [ ! -f ./compile ];then 
	sh setup.sh
fi

./configure

make

cd examples

../src/trident mir1.fasta mir1idealizedpurinestrand5to3.fasta -out test -json test.json
diff test test.out
python test_json.py test.json test.json.expected

cd ..
cd python

python setup.py build

PYTHONPATH=$PWD scripts/energy_score_ranker -o test/ranked.out test/score_count.raw
diff test/ranked.out test/ranked.out.expected
diff top_25-percent.dat test/top_25-percent.dat.expected

PYTHONPATH=$PWD scripts/energy_score_combiner -o test/combiner.out test/score_count.{1,2,3,4}
diff test/combiner.out test/score_count.raw

PYTHONPATH=$PWD scripts/energy_score_interpolator -o test/interpolator.sav test/ranked.out.expected
PYTHONPATH=$PWD scripts/energy_score_classifier -f test/interpolator.sav -c test/classes.dat -i test/score.out -p test/class_ --permissive
for i in 1 2 3 4 5;do
	diff test/class_$i test/class_${i}.expected
done


