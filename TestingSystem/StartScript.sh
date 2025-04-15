#!/bin/bash

cd Plot
python3 -m venv venv
source venv/bin/activate
pip install matplotlib
cd ..
make
make run
cd Plot
mkdir -p img
python PlotBuilder.py
deactivate
open img/TestsResult.png
