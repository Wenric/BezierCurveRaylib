#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>
#include <iostream>

// pascal triangle
std::vector<std::vector<int>> compute_pascals_triangle(int R) {
    std::vector<std::vector<int>> pasc(R + 1, std::vector<int>(R + 1, 0));

    for (int i = 0; i <= R; ++i) {
        pasc[i][0] = 1;
        pasc[i][i] = 1;
    }

    for (int i = 2; i <= R; ++i) {
        for (int j = 1; j < i; ++j) {
            pasc[i][j] = pasc[i - 1][j - 1] + pasc[i - 1][j];
        }
    }
    return pasc;
}

// computation for points
Vector2 compute_bezier_point(std::vector<Vector2>& control_points, int R, float t, std::vector<std::vector<int>>& pascals_triangle) {
    Vector2 point = {0, 0};
    for (int i = 0; i <= R; ++i) {
        float adjustment = pascals_triangle[R][i] * std::pow(t, i) * std::pow(1 - t, R - i);
        point.x += control_points[i].x * adjustment;
        point.y += control_points[i].y * adjustment;
    }
    return point;
}

int main() {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int FPS = 60;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Parametric Bezier Curve");
    SetTargetFPS(FPS);

    // user-input
    int R;
    std::cout << "Enter the degree of the curve: " << std::endl;
    std::cin >> R;

    std::vector<std::vector<int>> pascals_triangle = compute_pascals_triangle(R);

    int num_steps;
    std::cout << "Enter the number of steps: " << std::endl;
    std::cin >> num_steps;

    int num_control_points;
    std::cout << "Enter the number of control points: " << std::endl;
    std::cin >> num_control_points;

    std::vector<Vector2> control_points(num_control_points);
    for (int i = 0; i < num_control_points; ++i) {
        std::cout << "Enter control point " << i << " coordinates: " << std::endl;
        std::cin >> control_points[i].x >> control_points[i].y;
    }

    int selected_point = -1;

    while (!WindowShouldClose()) {
        //detect which control point is selected
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse_pos = GetMousePosition();
            for (int i = 0; i < num_control_points; ++i) {
                if (CheckCollisionPointCircle(mouse_pos, control_points[i], 8)) {
                    selected_point = i;
                    break;
                }
            }
        }

        // position handling
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selected_point != -1) {
            control_points[selected_point] = GetMousePosition();
        } else {
            selected_point = -1;
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //drawing the bezier curve
        for (int i = 0; i < num_steps; ++i) {

            float t1 = static_cast<float>(i) / num_steps;
            float t2 = static_cast<float>(i + 1) / num_steps;
            Vector2 p1 = compute_bezier_point(control_points, R, t1, pascals_triangle);
            Vector2 p2 = compute_bezier_point(control_points, R, t2, pascals_triangle);
            DrawLine(p1.x, p1.y, p2.x, p2.y, BLACK);
        }

        // drawing the control points
        for (int i = 0; i < control_points.size(); i++) {
            DrawCircle(control_points[i].x, control_points[i].y, 8, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}