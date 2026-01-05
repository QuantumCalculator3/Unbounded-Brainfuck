import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class unboundedbf{
	public static void main(String[] args){
		if (args.length!=1){
			System.out.println("Error: This interpreter takes one argument, which must be the path to brainfuck source code.");
			System.exit(1);
		}

		Scanner inputs = new Scanner(System.in);
		File source = new File(args[0]);
		char[] code = new char[(int) source.length()];

		try (FileReader fr = new FileReader(source)){
			fr.read(code);
		}
		catch (IOException e){
			System.out.println("Error: Failed to read file.");
			System.exit(1);
		}

		int dataPointer = 0;
		ArrayList<Integer> data = new ArrayList<Integer>();
		for (int i=0;i<30000;i++){
			data.add(0);
		}

		for (int i=0;i<code.length;i++){
			if (code[i]=='>'){
				dataPointer++;
				if (dataPointer>data.size()){
					for (int j=0;j<100;j++){
						data.add(0);
					}
				}
			}
			else if (code[i]=='<'){
				dataPointer--;
				if (dataPointer<0){
					for (int j=0;j<100;j++){
						data.add(0,0);
					}
					dataPointer+=100;
				}
			}
			else if (code[i]=='+'){
				data.set(dataPointer, data.get(dataPointer)+1);
				if (data.get(dataPointer)>127){
					data.set(dataPointer,0);
				}
			}
			else if (code[i]=='-'){
				data.set(dataPointer, data.get(dataPointer)-1);
				if (data.get(dataPointer)<0){
					data.set(dataPointer, 127);
				}
			}
			else if (code[i]=='.'){
				System.out.printf("%c",data.get(dataPointer));
			}
			else if (code[i]==','){
				String line = inputs.nextLine();

				if (line.isEmpty()){
					data.set(dataPointer, 0);
				}
				else{
					char input = line.charAt(0);
					if (input>127||input<0){
						System.out.println("Error: Invalid input.");
						System.exit(1);
					}
					data.set(dataPointer, (int) input);
				}
			}
			else if (code[i]=='['){
				if (data.get(dataPointer)==0){
					int opencount = 1;
					while (code[i]!=']'||opencount!=0){
						i++;
						if (i==code.length){
							System.out.println("Error: No matching ] found.");
							System.exit(1);
						}
						if (code[i]=='['){
							opencount++;
						}
						if (code[i]==']'){
							opencount--;
						}
					}
				}
			}
			else if (code[i]==']'){
				if (data.get(dataPointer)!=0){
					int closecount = 1;
					while (code[i]!='['||closecount!=0){
						i--;
						if (i<0){
							System.out.println("Error: No matching [ found.");
							System.exit(1);
						}
						if (code[i]=='['){
							closecount--;
						}
						if (code[i]==']'){
							closecount++;
						}
					}
					i--;
				}
			}
		}
		
		System.exit(0);
	}
}
