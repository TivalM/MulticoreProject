In the folder mulmatrix and countsort, there are three files of source code: *_seq.c for the sequential version, *_omp.c for the OpenMp version, and *_pth.c for the Pthread version.

running ./auto.sh on the cluster will automatically do experiments for the three versions of the algorithms and ouput the execution times to *.log files.

After downloading the *.log files, running the data_processing.ipynb to get the average execution times, then copying the data to excel files and getting the plots.