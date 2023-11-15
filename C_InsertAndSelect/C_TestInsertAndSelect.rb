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
      ".exit",
    ])
    expect(result).to match_array([
      "db > Syntax error. Could not parse statement.",
      "db > Bye!",
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
