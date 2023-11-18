describe 'database' do
    def run_script(commands)
        raw_output = nil
        IO.popen("./D_StoreToDiskWithTests", "r+") do |pipe|
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
end