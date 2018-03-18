defmodule Niftest do
 @on_load :init
  def init() do
    :ok = :erlang.load_nif("./niftest", 0)
  end

  def hello() do
    "NIF library not loaded"
  end

end
