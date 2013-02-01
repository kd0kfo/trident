import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;

public class TridentScoreMR extends Configured implements Tool{
	public static final String field_delim = ":";
	public static final String default_fields = Integer.toString(TridentMapper.energy_token) + field_delim + Integer.toString(TridentMapper.score_token);
	public static String join_fields(String[] ids){return join_fields(ids,1);}
	public static String join_fields(String[] ids,int offset)
	{
		int len = ids.length - offset;

		if(len == 0)
			return null;
		String fields = "";
		for(int i = 0;i<len;i++)
			fields += ":"+ids[i+offset];
		if(fields.length() > 0)
			if(fields.charAt(0) == ':')
				fields = fields.substring(1);
		return fields;
	}
	
	public static void main(String[] args) throws Exception
	{
		int retval = ToolRunner.run(new TridentScoreMR(),args);
		System.exit(retval);
	}
	
	public static Path get_output_path(String path)
	{
		Date date = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddHHmmss");
		String time_stamp = sdf.format(date); 
		
		return new Path(path + ".score_count." + time_stamp);
	}
	

	@Override
	public int run(String[] args) throws Exception {
		
		if(args.length == 0)
		{
			System.err.println("Usage: TridentScoreMR <input> [field numbers]");
			System.exit(-1);
		}
		String path = args[0];
		
		// Setup Configuration
		Configuration config = getConf();
		if(config == null)
		{
			System.err.println("NULL Configuration!");
			System.exit(42);
		}
		config.setInt("score_length", 15);
		
		// Configuration
		if(args.length > 1)
		{
			String field_ids = join_fields(args);
			if(field_ids == null)
			{
				System.out.println("Fields list is null");
			}
			config.set("map_fields", field_ids);
			config.set("field_delim",field_delim);
		}
		
		// Setup Job
		Job job = new Job(config);
		job.setJarByClass(TridentScoreMR.class);
		job.setJobName("Count Scores");
		if(args.length > 1)
		{			
			job.setMapperClass(TridentGenericMapper.class);
		}	
		else
		{
			job.setMapperClass(TridentMapper.class);
		}

		
		// Input Path
		FileInputFormat.addInputPath(job, new Path(path));
		
		// Output Path (+timestamp)
		FileOutputFormat.setOutputPath(job, get_output_path(path));

		job.setCombinerClass(TridentReducer.class);

		job.setNumReduceTasks(4);
		job.setReducerClass(TridentReducer.class);

		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);

		if(job.waitForCompletion(true))
			return 0;
		
		return 1;
	}

}