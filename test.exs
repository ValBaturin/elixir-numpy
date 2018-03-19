defmodule Test do
    import Numpy
    def deepcopy do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        result = make_deep_list make_matrix(non_sq_matrix)
        IO.puts("\nDEEP COPY")
        IO.puts("input:")
        IO.inspect non_sq_matrix
        IO.puts("output")
        IO.inspect result
    end

    def matrixsum do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        non_sq_matrix2 = [[0.0, 1.0, 6.0], [5.5, 3.2, 2.5]]
        result = msum non_sq_matrix, non_sq_matrix2
        IO.puts("\nMATRIX SUM")
        IO.puts("input:")
        IO.inspect non_sq_matrix
        IO.inspect non_sq_matrix2
        IO.puts("output")
        IO.inspect result
    end

    def matrixscale do
        scaler = 15.0
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        result = mscale scaler, non_sq_matrix
        IO.puts("\nMATRIX SUM")
        IO.puts("input:")
        IO.inspect scaler
        IO.inspect non_sq_matrix
        IO.puts("output")
        IO.inspect result
    end

    def col do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        col = 0
        IO.puts("\nGET COL")
        IO.puts("input:")
        IO.inspect col
        IO.inspect non_sq_matrix
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        IO.puts("output:")
        IO.inspect result
        col = 1
        IO.puts("input:")
        IO.inspect col
        IO.inspect non_sq_matrix
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        IO.puts("output:")
        IO.inspect result
         col = 2
        IO.puts("input:")
        IO.inspect col
        IO.inspect non_sq_matrix
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        IO.puts("output:")
        IO.inspect result
    end

    def test() do
        deepcopy()
        matrixsum()
        matrixscale()
        col()
    end
end
Test.test()
