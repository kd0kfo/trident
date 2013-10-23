package org.stjude.trident.hadoop;
import org.apache.hadoop.mapreduce.Mapper.Context;

public class TridentException extends Exception
{
    /**
	 * For Serializable
	 */
	private static final long serialVersionUID = 4865838519552794711L;

	static enum ErrorTypes{BROKEN_LINE,EMPTY_LINE};
    public TridentException(String msg){super(msg);}

    public static void bad_score(String score, Context context)
    {
    	System.err.println("Invalid score: " + score);
    	if(score.length() == 0)
    		context.getCounter(ErrorTypes.EMPTY_LINE).increment(1);
    	else
    		context.getCounter(ErrorTypes.BROKEN_LINE).increment(1);
    }
    
}
