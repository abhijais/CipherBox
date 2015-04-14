# CipherBox
CipherBox encrypts input_text_file and decrypts the encrypted one.

What is CipherBox?
		CipherBox is a free command line tool to encrypt your text files and
		decrypt the encrypted one. It's quiet useful if you want to hide the
		information from someone.
		Right now CipherBox supports en/decryption through "Hill Cipher"
		algorithm developed by Mr. Lester S. Hill in 1929.
		It maybe possible if author would add support for other en/decryption
		algorithms too.
		
Why is CipherBox made?
		There was a situation. Author have had to submit a minor project in mid of
		April in 2015. The technology he was fairly familiar was C. That is
		why he wrote CipherBox in C. The codes written by him are based on a
		unified philosophy "the code must be easy to read and understand"
		so if you are expecting a "faster software" with O(1) time complexity
		then sorry, this is not for you. BTW if you are interested....
		...The complexity of CipherBox is O(n).
		
How to use it?
		CipherBox is a command line utility, i.e., there is no GUI. It's geeky.
		It's not like those retro C programs where they created interfaces in
		Command Prompt / DOS window for user interaction, in fact the user will
		not have any interaction with CipherBox at all. Remember, CipherBox is
		just an utility program (just like mkdir, mov, del), you use it when
		you need it and nothing more. When you use mkdir to create a directory
		what you do?
			>mkdir new_folder
		And that's all, mkdir creates a new directory named new_folder and your
		prompt returns back to
			>
		and you are left with what you wanted.
		Similar is CipherBox.
		There are currently two commands available -
		 hillencrypt & hilldecrypt
		
		hillencrypt
		***********
		                   ###############  ,-------> encryption_of_input.txt
		  input.txt------> # hillencrypt #<
		                   ###############  `-------> key_of_input.txt
						   
						   
		  input.txt----\   ###############
						         > # hillencrypt # ---------> encryption_of_input.txt
		       keys----/   ###############
		  
		  Synopsis -
			hillencrypt
			hillencrypt input_file_name
			hillencrypt -i input_file_name
			hillencrypt -k user_provided_keys
			hillencrypt -i input_file_name -k user_provided_keys
			hillencrypt -k user_provided_keys -i input_file_name
		
		hilldecrypt
		***********
		 encryption_of_input.txt-  ###############
		                          ># hilldecrypt #> decryption_of_encryption_of_input.txt
		        key_of_input.txt-  ###############
						   
						   
		     input_file_name.txt-  ###############
		                          ># hilldecrypt #> decryption_of_input_file_name.txt
		                    keys-  ###############
				 
		  Synopsis -
			hilldecrypt
			hilldecrypt input_file_name key_file_name
			hilldecrypt -i input_file_name key_file_name
			hilldecrypt input_file_name -k user_provided_keys
			hilldecrypt -i input_file_name -k user_provided_keys
			hilldecrypt -k user_provided_keys -i input_file_name
