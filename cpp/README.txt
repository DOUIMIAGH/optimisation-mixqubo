Compilation depuis la racine du dépôt :

cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build --parallel

Test de l’évaluation :
./cpp/build/src/test_mixQUBO instances/mxqubo_test.json

Étalonnage (30 000 évaluations sur la plus grande instance) :
./cpp/build/src/etalon instances/mxqubo_v0_15.json

Consulter le README.md à la racine pour les méthodes, les expériences et l’analyse.
