defmodule Niftest do
 @on_load :init
  def init() do
    :erlang.load_nif("./niftest", 0)
    :ok
  end

  def hello() do
    "NIF library not loaded"
  end

end
