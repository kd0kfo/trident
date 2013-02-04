#!/usr/bin/env octave
# 
# energy_score_plots.m - Plot generating octave script for trident results.
#
# Expects a tab-delimited list of energy-score pairs, with the first three
# columns containing energy, score and frequency, respectively.
#
# Command line arguments: filename and optional title (string in quotation marks).
#
# If the scale_by_two variable is non-zero, the frequency values are divided by two.

scale_by_two = 0;

function retval = extract_data(matrix,column)
  retval = horzcat(matrix(:,column),matrix(:,3));
  retval = reduce_matrix(retval);
  retval = sortrows(retval,1);
endfunction

the_title = "Frequency";
arg_list = argv();
if nargin < 1
  printf("Needs a sorted score file.\n");
  exit(42);
endif

if nargin > 1
  the_title = arg_list{2};
endif

infile = arg_list{1};

data = dlmread(infile,"\t",1,0);

# Scale data?
if scale_by_two
  printf("REDUCED COUNT BY 2!!!!!!!\n");
  if min(data(:,3)) == 1
    printf("MINIMUM VALUE WAS 1!!!!\n");
  endif
  data(:,3) /= 2;
endif

# Energy data
reduced_data = extract_data(data,1);
bar(reduced_data(:,1),reduced_data(:,2));
xlabel("Energy (kcal)");ylabel("Frequency");title(the_title);
saveas(1,"energy.png");

# Score data
reduced_data = extract_data(data,2);
bar(reduced_data(:,1),reduced_data(:,2));
xlabel("Score");ylabel("Frequency");title(the_title);
saveas(1,"score.png");


