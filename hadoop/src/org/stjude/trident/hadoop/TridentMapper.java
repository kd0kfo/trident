package org.stjude.trident.hadoop;

import java.io.IOException;
import java.text.DecimalFormat;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class TridentMapper
    extends Mapper<LongWritable, Text, Text, IntWritable>{

    public static final int score_token = 2;// index of score in score tokens
    public static final int energy_token = 3;// index of energy in score tokens

    @Override
	public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException{
	IntWritable newval = new IntWritable(1);
	String score = value.toString().trim();
	Configuration config = context.getConfiguration();
	int score_length = config.getInt("score_token", 15);
	
	if(score.length() == 0)
	    return;

	String[] tokens = score.split(",");

	if(tokens.length < score_length)
	{
		TridentException.bad_score(score,context);
		return;
	}

	DecimalFormat df = new DecimalFormat();
	df.setMaximumFractionDigits(1);
	double energy_val;
	try{
		energy_val = Double.valueOf(tokens[energy_token]);
	}
	catch(NumberFormatException nfe)
	{
		System.err.println("Could not convert energy value to double.");
		TridentException.bad_score(score,context);
		return;
	}
	String energy = df.format(energy_val).toString();
	String score_val = tokens[score_token];


	context.write(new Text(energy + ":" + score_val), new IntWritable(1));
    }
}
