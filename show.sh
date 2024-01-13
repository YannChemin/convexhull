tail -n +2 input.csv | awk -F',' '{print $1, $2}' > input.dat

#gnuplot -persist -e "plot 'input.dat' with points title 'Input Data'"
#gnuplot -persist -e "plot 'convex_hull_output.dat' with linespoints"
#gnuplot -persist -e "plot 'input.dat' with points title 'Input Data', 'convex_hull_output.dat' with linespoints title 'Convex Hull'"
#gnuplot -persist -e "set style line 1 lc rgb 'brown' lw 2; plot 'input.dat' with lines linestyle 1 title 'Input Data'"
#gnuplot -persist -e "set style line 1 lc rgb 'brown' lw 2; plot 'input.dat' with lines linestyle 1 title 'Input Data', 'convex_hull_output.dat' with linespoints title 'Convex Hull'"
#gnuplot -persist -e "set style line 1 lc rgb 'brown' lw 2; plot 'input.dat' with lines linestyle 1 title 'Input Data', 'convex_hull_interpolated_output.dat' with linespoints title 'Convex Hull'"
gnuplot -persist -e "set style line 1 lc rgb 'brown' lw 2; plot 'cr_output.dat' with linespoints title 'Continuum Removed'"


