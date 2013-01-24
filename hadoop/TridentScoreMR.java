import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class TridentScoreMR{

    public static void main(String[] args) throws Exception{
	if(args.length != 1)
	    {
		System.err.println("Usage: TridentScoreMR <input>");
		System.exit(-1);
	    }
	String path = args[0];
	Job job = new Job();
	job.setJarByClass(TridentScoreMR.class);
	job.setJobName("Count Scores");
	
	FileInputFormat.addInputPath(job, new Path(path));
	FileOutputFormat.setOutputPath(job, new Path(path + ".score_count"));
	
	job.setMapperClass(TridentMapper.class);
	job.setCombinerClass(TridentReducer.class);
	
	job.setNumReduceTasks(4);
	job.setReducerClass(TridentReducer.class);

	job.setOutputKeyClass(Text.class);
	job.setOutputValueClass(IntWritable.class);
    
	System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}