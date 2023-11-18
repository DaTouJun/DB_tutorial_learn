describe 'database' do
    it 'test exit and unrecognized command and sql sentence' do
        result = run_script([
          "hello world",
          ".HELLO WORLD",
          ".exit",
        ])
        expect(result).to match_array([
          "db > Unrecognized keyword at start of 'hello world'.",
          "db > Unrecognized command:.HELLO WORLD",
          "db > Bye!",
        ])
    end

    it 'inserts and retrieves a row' do
        result = run_script([
            "insert 1 user1 person1@example.com",
            "select",
            ".exit",
        ])
        expect(result).to match_array([
            "db > Executed.",
            "db > (1, user1, person1@example.com)",
            "Executed.",
            "db > Bye!",
        ])
    end

    it 'error insert' do
        result = run_script([
            "insert 2 user2",
            ".exit"
        ])
        expect(result).to match_array([
          "db > Syntax error. Could not parse statement.",
          "db > Bye!",
        ])
    end

    # �������ݿ��������
    it 'prints error message when table is full' do
        script  = (1..1401).map do |i|
            "insert #{i} user#{i} person#{i}@example.com"
        end
        script << ".exit"
        result = run_script(script)
        expect(result[-2]).to eq('db > Error : Table full.')
    end

    # �������Եı߽����
    it 'allows inserting string that are the maximum length' do
        long_username = "a"*32
        long_email = "a" * 255
        script = [
            "insert 1 #{long_username} #{long_email}",
            "select",
            ".exit"
        ]
        result = run_script(script)
        expect(result).to match_array([
            "db > Executed.",
            "db > (1, #{long_username}, #{long_email})",
            "Executed.",
            "db > Bye!"
        ])
    end

    # ��������̫��������
    it 'prints error message if string are too long' do
        long_username = "a"*33
        long_email = "a"*256
        script = [
            "insert 1 #{long_username} #{long_email}",
            "select",
            ".exit"
        ]
        result = run_script(script)
        expect(result).to match_array([
            "db > String is too long.",
            "db > Executed.",
            "db > Bye!"
        ])
    end

    # ���ID�Ǹ�ֵ����
    it 'prints an error message if id is negative' do
        script = [
            "insert -1 datoujun datou@d.com",
            "select",
            ".exit"
        ]
        result = run_script(script)
        expect(result).to match_array([
            "db > ID must be positive.",
            "db > Executed.",
            "db > Bye!"
        ])
    end

    def run_script(commands)
        raw_output = nil
        IO.popen("./C_insertAndSelect", "r+") do |pipe|
            commands.each do |command|
            pipe.puts command
        end

        pipe.close_write
        # Read entire output
        raw_output = pipe.gets(nil)
        end
    raw_output.split("\n")
  end

end
