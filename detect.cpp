#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    out->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string ask_ollama(const std::string& prompt) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string json =
            "{"
            "\"model\":\"llama3.2:latest\","
            "\"prompt\":\"" + prompt + "\","
            "\"stream\":false"
            "}";

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}

std::string describe_scene(const std::vector<cv::Vec4i>& lines) {
    if (lines.empty()) {
        return "No lane lines detected.";
    }
    else if (lines.size() < 5) {
        return "Few lane lines detected, road may be unclear.";
    }
    else {
        return "Multiple lane lines detected, road is likely clear.";
    }
}

int main() {
    cv::VideoCapture cap("C:\\Users\\WINDOWS\\projects\\Lane_Detection\\road.mp4");

    if (!cap.isOpened()) {
        std::cout << "Error opening video stream\n";
        return -1;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);

    int frame_count = 0;

    while (true) {
        cv::Mat frame, gray, blur, edges, mask, result;

        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blur, cv::Size(9, 9), 1.0);
        cv::Canny(blur, edges, 100, 120);

        mask = cv::Mat::zeros(edges.size(), edges.type());

        int w = edges.cols;
        int h = edges.rows;

        std::vector<cv::Point> roi{
            cv::Point(150, h),
            cv::Point(w - 150, h),
            cv::Point(w - 400, h * 0.53),
            cv::Point(400, h * 0.53)
        };

        cv::fillConvexPoly(mask, roi, 255);

        cv::Mat masked;
        cv::bitwise_and(edges, mask, masked);

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(masked, lines, 1, CV_PI / 180, 50, 50, 10);

        result = frame.clone();
        for (auto& l : lines) {
            cv::line(result,
                cv::Point(l[0], l[1]),
                cv::Point(l[2], l[3]),
                cv::Scalar(0, 0, 255), 3);
        }

        cv::imshow("Lane Detection", result);

        if (frame_count % 30 == 0) {  
            std::string scene = describe_scene(lines);

            std::string prompt =
                "You are a driving assistant. Explain the road scene briefly: " + scene;

            std::string ai = ask_ollama(prompt);

            std::cout << "\nAI: " << ai << std::endl;
        }

        frame_count++;

        char c = (char)cv::waitKey(25);
        if (c == 27) break;
    }

    curl_global_cleanup();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}