package com.ezradavis.www.romannumeral;

public class RomanNumeral {
    static int[] romanInts = {1000, 500, 100, 50, 10, 5, 1, 0, 0 };

    static String[] romanStrings = {"M", "D", "C", "L", "X", "V", "I", "Fail", "Fail2"};
    
    // constructor
    RomanNumeral (){
   }
    
    
    
    // intToRoman
    static public String intToRoman (int integer){
    	String romanString = "";
    	while (integer>=1){
      	  //Ugly debug
      	  //System.out.println("while iteration");
      	  //System.out.println(romanString);
      	  //System.out.println(integer);
          for(int i=0; i<romanInts.length; i++) {
        	       	  
        	  if(integer>=romanInts[i]) {
        		  romanString = romanString.concat(romanStrings[i]);
        		  integer -= romanInts[i];
        		  break;
        	  } else if((integer >= (romanInts[i] - romanInts[i+1])) && (romanInts[i] == 5 * romanInts[i+1])){
        		  // For IV and the like
        		  romanString += romanStrings[i+1]+ romanStrings[i];
        		  integer = integer + romanInts[i+1] - romanInts[i];
        		  break;
        	  } else if((integer >= (romanInts[i] - romanInts[i+2])) && (romanInts[i] == 10 * romanInts[i+2])){
        		  // For IX and the like
        		  romanString += romanStrings[i+2]+ romanStrings[i];
        		  integer = integer + romanInts[i+2] - romanInts[i];
        		  break;
        	  }
          }
    	}
          return romanString;
      }
    
    // duh
    static int romanToInt (String roman) {
    	int decimal = 0;
          for(int i=0; i<roman.length()-1; i++){
        	  int value = romanCharMatch(roman.substring(i,i+1));
        	  
        	  //bad recomputation... but whatever
        	  int nextValue = romanCharMatch(roman.substring(i+1,i+2));
        	  
        	  if(nextValue>value){
        		  decimal += nextValue - value;
        		  i++;
        	  } else {
        		  decimal += value;
        	  }
          }
          decimal += romanCharMatch(roman.substring(roman.length() - 1));
          return decimal;
    }
    
    static private int romanCharMatch(String character){
    	int decimal = 0;
    	for(int j=0; j<romanStrings.length; j++){
    		if (romanStrings[j].equals(character)) {
   			 
   			 decimal = romanInts[j];
   					 break;
    		}
    	}
    	return decimal;
    }
    
    
    
    
    public static void main(String[] args) {
    	    //boring test
    		System.out.println(RomanNumeral.intToRoman(35));
    		System.out.println("\n");
    		System.out.println(RomanNumeral.intToRoman(19));
    		System.out.println("\n");
    		System.out.println(RomanNumeral.intToRoman(4));
    		
    		System.out.println(RomanNumeral.intToRoman(494));
    		
    		System.out.println(RomanNumeral.intToRoman(50148));
    		
    		System.out.println(RomanNumeral.romanToInt("X"));
    		
    		System.out.println(RomanNumeral.romanToInt("XCVIII"));
    		
    		System.out.println(RomanNumeral.romanToInt("CMVI"));
    		

    }
}



