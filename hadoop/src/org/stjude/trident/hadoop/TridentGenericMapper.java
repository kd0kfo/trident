package org.stjude.trident.hadoop;
import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Mapper.Context;


public class TridentGenericMapper extends
		Mapper<LongWritable, Text, Text, IntWritable> {
	
	static enum MapperErrors{MISSING_FIELDS,TRUNCATED_SCORE_LINE};
	
	public static int[] parse_fields(String fields,String delim)
	{
		if(fields == null)
			return null;
		
		StringTokenizer tokens = new StringTokenizer(fields,delim);
		int len = tokens.countTokens();
		if(len == 0)
			return null;
		
		int[] field_list = new int[len];
		int counter = 0;
		while(tokens.hasMoreTokens())
		{
			String field = tokens.nextToken();
			try
			{
				field_list[counter] = Integer.parseInt(field);
			}
			catch(NumberFormatException nfe)
			{
				System.err.println("TridentGenericMapper: Invalid field to be used \"" + field +"\"");
				throw nfe;
			}
			counter++;
		}
		
		return field_list;
	}
	
	@Override
	public void map(LongWritable key, Text value, Context context) 
			throws IOException, InterruptedException{
		
		IntWritable newval = new IntWritable(1);
		String score = value.toString().trim();
		Configuration config = context.getConfiguration();
		int score_length = config.getInt("score_token", 15);
		String field_delim = config.get("field_delim");
		int[] field_list = parse_fields(config.get("map_fields"),field_delim);
		String key_out = "";
		
		if(field_list == null || field_list.length == 0)
		{
			context.getCounter(MapperErrors.MISSING_FIELDS).increment(1);
			return;
		}
		
		if(score.length() == 0)
		    return;

		String[] tokens = score.split(",");

		if(tokens.length < score_length)
		{
			TridentException.bad_score(score,context);
			return;
		}
		
		int num_fields = field_list.length;
		for(int i = 0;i<num_fields;i++)
		{
			if(field_list[i] >= tokens.length)
			{
				System.err.println("TridentGenericMapper: Invalid field \"" + Integer.toString(field_list[i]) + "\" is greater than number of score tokens \"" + Integer.toString(tokens.length) + "\"");
				System.err.println("Score used: " + score);
				context.getCounter(MapperErrors.TRUNCATED_SCORE_LINE).increment(1);
			}
			key_out += ":" + tokens[field_list[i]];
		}
		if(key_out.length() > 0)
			key_out = key_out.substring(1);
		
		context.write(new Text(key_out), newval);
		
	}

		
}
