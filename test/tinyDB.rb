describe 'database' do
    def run_script(commands)
        raw_output = nil
        IO.popen("./D_StoreToDisk test.db", "r+") do |pipe|
            commands.each do |command|
            pipe.puts command
        end
        pipe.close_write
        raw_output = pipe.gets(nil)
        end
        raw_output.split("\n")
    end

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

    it "test storage function" do
        result1 = run_script([
            "insert 1 user1 person1@example.com",
            ".exit",
        ])
        expect(result1).to match_array([
            "db > Executed.",
            "db > Bye!",
        ])
        result2 = run_script([
            "select",
            ".exit",
        ])
        expect(result2).to match_array([
            "db > (1, user1, person1@example.com)",
            "Executed.",
            "db > Bye!",
        ])
    end
end